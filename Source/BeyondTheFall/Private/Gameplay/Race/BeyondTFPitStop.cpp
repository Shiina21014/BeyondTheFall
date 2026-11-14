// Fill out your copyright notice in the Description page of Project Settings.
#include "BeyondTFPitStop.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Framework/BeyondTFRaceGameState.h"
#include "Framework/BeyondTFRacePlayerState.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"

ABeyondTFPitStop::ABeyondTFPitStop()
{
	bReplicates = true;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	RootComponent = DetectionBox;

	DetectionBox->SetBoxExtent(FVector(150.0f, 150.0f, 60.0f));
	DetectionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionBox->SetCollisionObjectType(ECC_WorldDynamic);
	DetectionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectionBox->SetGenerateOverlapEvents(true);

	DetectionBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ABeyondTFPitStop::OnPitOverlap
	);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("PlatformMesh")
	);
	PlatformMesh->SetupAttachment(DetectionBox);
	PlatformMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	PlatformMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.2f));
	PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PitSign = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PitSign"));
	PitSign->SetupAttachment(DetectionBox);
	PitSign->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	PitSign->SetHorizontalAlignment(EHTA_Center);
	PitSign->SetWorldSize(45.0f);
	PitSign->SetTextRenderColor(FColor::Green);
	PitSign->SetText(FText::FromString(TEXT("PARADA DE PITS")));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		PlatformMesh->SetStaticMesh(CubeMesh.Object);
	}

}

void ABeyondTFPitStop::OnPitOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!HasAuthority())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	ABeyondTFRacePlayerState* RacePlayerState =
		Character->GetPlayerState<ABeyondTFRacePlayerState>();

	ABeyondTFRaceGameState* RaceGameState =
		GetWorld()->GetGameState<ABeyondTFRaceGameState>();

	if (!RacePlayerState || !RaceGameState)
	{
		return;
	}

	const bool bRaceStarted =
		RaceGameState->RacePhase == ERacePhase::Racing
		|| (RaceGameState->RacePhase == ERacePhase::Countdown
			&& RaceGameState->CountdownNumber == 0);

	if (
		!bRaceStarted
		|| RacePlayerState->bDisqualified
		|| RacePlayerState->bFinishedRace
	)
	{
		return;
	}

	const TWeakObjectPtr<ABeyondTFRacePlayerState> PlayerKey(RacePlayerState);

	if (PlayersWhoUsedPit.Contains(PlayerKey))
	{
		return;
	}

	if (RacePlayerState->AttemptsRemaining >= 4)
	{
		return;
	}
	const int32 RestoredAttempts = FMath::Clamp(AttemptsToRestore, 1, 2);
	const int32 PreviousAttempts = RacePlayerState->AttemptsRemaining;

	RacePlayerState->AttemptsRemaining = FMath::Min(
		4,
		PreviousAttempts + RestoredAttempts
	);

	const int32 ActualRestored =
		RacePlayerState->AttemptsRemaining - PreviousAttempts;

	PlayersWhoUsedPit.Add(PlayerKey);
	RacePlayerState->ClientAttemptsRestored(ActualRestored);

}

