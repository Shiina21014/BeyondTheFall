#include "BeyondTFRaceGM.h"

#include "Framework/BeyondTFRaceGameState.h"
#include "Framework/BeyondTFRacePlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Gameplay/Race/BeyondTFCheckpoint.h"
#include "Gameplay/Race/BeyondTFRaceRoute.h"
#include "Components/SplineComponent.h"
#include "EngineUtils.h"


ABeyondTFRaceGM::ABeyondTFRaceGM()
{
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = ABeyondTFRaceGameState::StaticClass();
	PlayerStateClass = ABeyondTFRacePlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> CharacterClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")
	);

	if (CharacterClass.Succeeded())
	{
		DefaultPawnClass = CharacterClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonPlayerController")
	);

	if (ControllerClass.Succeeded())
	{
		PlayerControllerClass = ControllerClass.Class;
	}
}

void ABeyondTFRaceGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState)
	{
		return;
	}

	const bool bRaceStarted =
		RaceGameState->RacePhase == ERacePhase::Racing
		|| (RaceGameState->RacePhase == ERacePhase::Countdown
			&& RaceGameState->CountdownNumber == 0);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		ACharacter* Character = PlayerController
			? Cast<ACharacter>(PlayerController->GetPawn())
			: nullptr;

		if (!Character)
		{
			continue;
		}

		ABeyondTFRacePlayerState* RacePlayerState =
			Character->GetPlayerState<ABeyondTFRacePlayerState>();

		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

		if (
	!bRaceStarted
	|| (RacePlayerState
		&& (RacePlayerState->bDisqualified || RacePlayerState->bFinishedRace))
)
		{
			if (Movement->MovementMode != MOVE_None)
			{
				Movement->StopMovementImmediately();
				Movement->DisableMovement();
			}
		}
		else if (Movement->MovementMode == MOVE_None)
		{
			Movement->SetMovementMode(MOVE_Walking);
		}
	}
	
	if (bRaceStarted)
	{
		PositionUpdateTime += DeltaSeconds;

		if (PositionUpdateTime >= 0.25f)
		{
			PositionUpdateTime = 0.0f;
			UpdateRacePositions();
		}
	}

}

void ABeyondTFRaceGM::StartCountdown()
{
	if (!HasAuthority())
	{
		return;
	}

	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState || RaceGameState->RacePhase != ERacePhase::Waiting)
	{
		return;
	}

	RaceGameState->RacePhase = ERacePhase::Countdown;
	RaceGameState->CountdownNumber = 3;

	GetWorldTimerManager().SetTimer(
		CountdownTimer,
		this,
		&ABeyondTFRaceGM::AdvanceCountdown,
		1.0f,
		true
	);
}

void ABeyondTFRaceGM::AdvanceCountdown()
{
	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimer);
		return;
	}

	if (RaceGameState->CountdownNumber > 0)
	{
		RaceGameState->CountdownNumber--;

		// Al aparecer ¡YA!, comienza el cronómetro.
		if (RaceGameState->CountdownNumber == 0)
		{
			RaceGameState->RaceTime = 0.0f;

			GetWorldTimerManager().SetTimer(
				RaceClockTimer,
				this,
				&ABeyondTFRaceGM::AdvanceRaceClock,
				1.0f,
				true
			);
		}

		return;
	}

	GetWorldTimerManager().ClearTimer(CountdownTimer);
	RaceGameState->RacePhase = ERacePhase::Racing;
}
void ABeyondTFRaceGM::AdvanceRaceClock()
{
	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState || RaceGameState->RacePhase == ERacePhase::Finished)
	{
		GetWorldTimerManager().ClearTimer(RaceClockTimer);
		return;
	}

	RaceGameState->RaceTime += 1.0f;
}
void ABeyondTFRaceGM::RegisterFinish(ACharacter* Character)
{
	if (!HasAuthority() || !Character)
	{
		return;
	}

	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	ABeyondTFRacePlayerState* RacePlayerState =
		Character->GetPlayerState<ABeyondTFRacePlayerState>();

	if (!RaceGameState || !RacePlayerState)
	{
		return;
	}

	const bool bRaceStarted =
		RaceGameState->RacePhase == ERacePhase::Racing
		|| (RaceGameState->RacePhase == ERacePhase::Countdown
			&& RaceGameState->CountdownNumber == 0);

	if (
		!bRaceStarted
		|| RacePlayerState->bFinishedRace
		|| RacePlayerState->bDisqualified
	)
	{
		return;
	}

	RacePlayerState->bFinishedRace = true;
	RacePlayerState->FinishPosition = NextFinishPosition;
	RacePlayerState->FinishTime = RaceGameState->RaceTime;
	RacePlayerState->ClientFinishReached();

	NextFinishPosition++;

	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->DisableMovement();
	CheckRaceEnd();
}
void ABeyondTFRaceGM::CheckRaceEnd()
{
	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState || RaceGameState->PlayerArray.IsEmpty())
	{
		return;
	}

	for (APlayerState* PlayerState : RaceGameState->PlayerArray)
	{
		ABeyondTFRacePlayerState* RacePlayerState =
			Cast<ABeyondTFRacePlayerState>(PlayerState);

		if (!RacePlayerState)
		{
			return;
		}

		if (!RacePlayerState->bFinishedRace && !RacePlayerState->bDisqualified)
		{
			return;
		}
	}

	GetWorldTimerManager().ClearTimer(RaceClockTimer);
	RaceGameState->RacePhase = ERacePhase::Finished;
}
void ABeyondTFRaceGM::UpdateRacePositions()
{
	ABeyondTFRaceGameState* RaceGameState =
		GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState)
	{
		return;
	}

	ABeyondTFRaceRoute* LeftRoute = nullptr;
	ABeyondTFRaceRoute* RightRoute = nullptr;
	ABeyondTFCheckpoint* LeftCheckpoint = nullptr;
	ABeyondTFCheckpoint* RightCheckpoint = nullptr;
	ABeyondTFCheckpoint* CommonCheckpoint = nullptr;

	for (TActorIterator<ABeyondTFRaceRoute> It(GetWorld()); It; ++It)
	{
		if (It->RouteId == 1)
		{
			LeftRoute = *It;
		}
		else if (It->RouteId == 2)
		{
			RightRoute = *It;
		}
	}

	for (TActorIterator<ABeyondTFCheckpoint> It(GetWorld()); It; ++It)
	{
		if (It->GetCheckpointNumber() == 1 && It->GetRouteId() == 1)
		{
			LeftCheckpoint = *It;
		}
		else if (It->GetCheckpointNumber() == 1 && It->GetRouteId() == 2)
		{
			RightCheckpoint = *It;
		}
		else if (It->GetCheckpointNumber() == 2 && It->GetRouteId() == 0)
		{
			CommonCheckpoint = *It;
		}
	}

	if (
		!LeftRoute || !RightRoute
		|| !LeftCheckpoint || !RightCheckpoint || !CommonCheckpoint
		|| !LeftRoute->RouteSpline || !RightRoute->RouteSpline
	)
	{
		return;
	}

	struct FPlayerProgress
	{
		ABeyondTFRacePlayerState* PlayerState = nullptr;
		float Progress = 0.0f;
		int32 Group = 0;
	};

	TArray<FPlayerProgress> Players;

	for (APlayerState* PlayerState : RaceGameState->PlayerArray)
	{
		ABeyondTFRacePlayerState* RacePlayerState =
			Cast<ABeyondTFRacePlayerState>(PlayerState);

		if (!RacePlayerState)
		{
			continue;
		}

		FPlayerProgress Entry;
		Entry.PlayerState = RacePlayerState;

		if (RacePlayerState->bFinishedRace)
		{
			Entry.Group = 2;
			Entry.Progress = static_cast<float>(-RacePlayerState->FinishPosition);
			Players.Add(Entry);
			continue;
		}

		if (RacePlayerState->bDisqualified)
		{
			Entry.Group = 0;
			Players.Add(Entry);
			continue;
		}

		Entry.Group = 1;

		APawn* Pawn = RacePlayerState->GetPawn();
		if (!Pawn)
		{
			Players.Add(Entry);
			continue;
		}

		ABeyondTFRaceRoute* SelectedRoute = nullptr;
		ABeyondTFCheckpoint* BranchCheckpoint = nullptr;

		if (RacePlayerState->SelectedRouteId == 1)
		{
			SelectedRoute = LeftRoute;
			BranchCheckpoint = LeftCheckpoint;
		}
		else if (RacePlayerState->SelectedRouteId == 2)
		{
			SelectedRoute = RightRoute;
			BranchCheckpoint = RightCheckpoint;
		}
		else
		{
			const FVector PlayerLocation = Pawn->GetActorLocation();

			const FVector LeftLocation =
				LeftRoute->RouteSpline->FindLocationClosestToWorldLocation(
					PlayerLocation,
					ESplineCoordinateSpace::World
				);

			const FVector RightLocation =
				RightRoute->RouteSpline->FindLocationClosestToWorldLocation(
					PlayerLocation,
					ESplineCoordinateSpace::World
				);

			if (
				FVector::DistSquared(PlayerLocation, LeftLocation)
				<= FVector::DistSquared(PlayerLocation, RightLocation)
			)
			{
				SelectedRoute = LeftRoute;
				BranchCheckpoint = LeftCheckpoint;
			}
			else
			{
				SelectedRoute = RightRoute;
				BranchCheckpoint = RightCheckpoint;
			}
		}

		USplineComponent* Spline = SelectedRoute->RouteSpline;

		const float RouteLength = Spline->GetSplineLength();

		const float BranchDistance = Spline->GetDistanceAlongSplineAtLocation(
			BranchCheckpoint->GetActorLocation(),
			ESplineCoordinateSpace::World
		);

		const float CommonDistance = Spline->GetDistanceAlongSplineAtLocation(
			CommonCheckpoint->GetActorLocation(),
			ESplineCoordinateSpace::World
		);

		const float PlayerDistance = Spline->GetDistanceAlongSplineAtLocation(
			Pawn->GetActorLocation(),
			ESplineCoordinateSpace::World
		);

		if (
			RouteLength <= KINDA_SMALL_NUMBER
			|| BranchDistance <= KINDA_SMALL_NUMBER
			|| CommonDistance <= BranchDistance
			|| RouteLength <= CommonDistance
		)
		{
			Players.Add(Entry);
			continue;
		}

		float StartDistance = 0.0f;
		float EndDistance = BranchDistance;
		int32 Segment = 0;

		if (RacePlayerState->CheckpointNumber == 1)
		{
			StartDistance = BranchDistance;
			EndDistance = CommonDistance;
			Segment = 1;
		}
		else if (RacePlayerState->CheckpointNumber >= 2)
		{
			StartDistance = CommonDistance;
			EndDistance = RouteLength;
			Segment = 2;
		}

		const float SegmentProgress = FMath::Clamp(
			(PlayerDistance - StartDistance) / (EndDistance - StartDistance),
			0.0f,
			1.0f
		);

		Entry.Progress = static_cast<float>(Segment) + SegmentProgress;
		Players.Add(Entry);
	}

	Players.Sort(
		[](const FPlayerProgress& A, const FPlayerProgress& B)
		{
			if (A.Group != B.Group)
			{
				return A.Group > B.Group;
			}

			if (A.Progress != B.Progress)
			{
				return A.Progress > B.Progress;
			}

			return A.PlayerState->GetPlayerId() < B.PlayerState->GetPlayerId();
		}
	);

	for (int32 Index = 0; Index < Players.Num(); Index++)
	{
		Players[Index].PlayerState->CurrentPosition = Index + 1;
	}
}

