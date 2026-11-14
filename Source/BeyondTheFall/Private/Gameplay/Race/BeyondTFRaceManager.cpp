#include "Gameplay/Race/BeyondTFRaceManager.h"

#include "Components/SceneComponent.h"
#include "Framework/BeyondTFRacePlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BeyondTFRaceGM.h"
#include "Framework/BeyondTFRaceGameState.h"

ABeyondTFRaceManager::ABeyondTFRaceManager()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void ABeyondTFRaceManager::BeginPlay()
{
	Super::BeginPlay();

	InitialRespawnTransform = GetActorTransform();
}

void ABeyondTFRaceManager::SetCheckpoint(
	AActor* Player,
	const FTransform& NewCheckpoint,
	int32 CheckpointNumber,
	int32 RouteId
)

{
	if (!HasAuthority())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(Player);
	if (!Character)
	{
		return;
	}

	ABeyondTFRacePlayerState* RacePlayerState =
		Character->GetPlayerState<ABeyondTFRacePlayerState>();

	if (!RacePlayerState || RacePlayerState->bDisqualified)
	{
		return;
	}

	if (CheckpointNumber != RacePlayerState->CheckpointNumber + 1)
	{
		return;
	}
	
	if (CheckpointNumber == 1)
	{
		if (RouteId != 1 && RouteId != 2)
		{
			return;
		}

		RacePlayerState->SelectedRouteId = RouteId;
	}

	RacePlayerState->RespawnTransform = NewCheckpoint;
	RacePlayerState->CheckpointNumber = CheckpointNumber;
	RacePlayerState->bHasCheckpoint = true;

	RacePlayerState->ClientCheckpointReached(CheckpointNumber);
}

void ABeyondTFRaceManager::RespawnPlayer(AActor* PlayerToRespawn)
{
	if (!HasAuthority())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(PlayerToRespawn);
	if (!Character)
	{
		return;
	}

	ABeyondTFRacePlayerState* RacePlayerState =
		Character->GetPlayerState<ABeyondTFRacePlayerState>();

	if (!RacePlayerState || RacePlayerState->bDisqualified)
	{
		return;
	}

	RacePlayerState->AttemptsRemaining--;
	RacePlayerState->TotalFalls++;
	RacePlayerState->ClientFallOccurred();

	if (RacePlayerState->AttemptsRemaining <= 0)
	{
		RacePlayerState->AttemptsRemaining = 0;
		RacePlayerState->bDisqualified = true;
		if (ABeyondTFRaceGameState* RaceGameState =
	GetWorld()->GetGameState<ABeyondTFRaceGameState>())
		{
			RacePlayerState->FinishTime = RaceGameState->RaceTime;
		}


		Character->GetCharacterMovement()->StopMovementImmediately();
		Character->GetCharacterMovement()->DisableMovement();

		RacePlayerState->ClientDisqualified();
		if (ABeyondTFRaceGM* RaceGameMode =
	GetWorld()->GetAuthGameMode<ABeyondTFRaceGM>())
		{
			RaceGameMode->CheckRaceEnd();
		}

		return;
	}

	FTransform RespawnTransform = InitialRespawnTransform;

	if (RacePlayerState->bHasCheckpoint)
	{
		RespawnTransform = RacePlayerState->RespawnTransform;
	}

	FVector RespawnLocation = RespawnTransform.GetLocation();
	RespawnLocation.Z += 100.0f;

	Character->GetCharacterMovement()->StopMovementImmediately();

	Character->SetActorLocationAndRotation(
		RespawnLocation,
		RespawnTransform.Rotator(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}