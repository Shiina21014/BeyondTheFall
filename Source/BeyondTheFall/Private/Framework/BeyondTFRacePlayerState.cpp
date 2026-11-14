#include "Framework/BeyondTFRacePlayerState.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"

ABeyondTFRacePlayerState::ABeyondTFRacePlayerState()
{
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(
		TEXT("/Game/Audio_SFX/Chekpoint.Chekpoint")
	);

	if (SoundAsset.Succeeded())
	{
		CheckpointSound = SoundAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> FallAsset(
	TEXT("/Game/Audio_SFX/Muerte.Muerte")
);

	if (FallAsset.Succeeded())
	{
		FallSound = FallAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> FinishAsset(
		TEXT("/Game/Audio_SFX/Cruzando_meta.Cruzando_meta")
	);

	if (FinishAsset.Succeeded())
	{
		FinishSound = FinishAsset.Object;
	}

}

void ABeyondTFRacePlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABeyondTFRacePlayerState, CheckpointNumber);
	DOREPLIFETIME(ABeyondTFRacePlayerState, AttemptsRemaining);
	DOREPLIFETIME(ABeyondTFRacePlayerState, TotalFalls);
	DOREPLIFETIME(ABeyondTFRacePlayerState, bDisqualified);
	DOREPLIFETIME(ABeyondTFRacePlayerState, bFinishedRace);
	DOREPLIFETIME(ABeyondTFRacePlayerState, FinishPosition);
	DOREPLIFETIME(ABeyondTFRacePlayerState, FinishTime);
	DOREPLIFETIME(ABeyondTFRacePlayerState, SelectedRouteId);
	DOREPLIFETIME(ABeyondTFRacePlayerState, CurrentPosition);
}

void ABeyondTFRacePlayerState::ClientCheckpointReached_Implementation(
	int32 ReachedCheckpoint
)
{
	CheckpointNoticeNumber = ReachedCheckpoint;

	if (GetWorld())
	{
		CheckpointNoticeEndTime = GetWorld()->GetTimeSeconds() + 3.0f;
	}

	if (CheckpointSound)
	{
		UGameplayStatics::PlaySound2D(this, CheckpointSound);
	}
}



void ABeyondTFRacePlayerState::ClientDisqualified_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (PlayerController)
	{
		PlayerController->SetIgnoreMoveInput(true);
	}
}
void ABeyondTFRacePlayerState::ClientAttemptsRestored_Implementation(
	int32 RestoredAttempts
)
{
	PitNoticeAttempts = RestoredAttempts;

	if (GetWorld())
	{
		PitNoticeEndTime = GetWorld()->GetTimeSeconds() + 3.0f;
	}

	if (CheckpointSound)
	{
		UGameplayStatics::PlaySound2D(this, CheckpointSound);
	}
}

void ABeyondTFRacePlayerState::ClientFallOccurred_Implementation()
{
	if (FallSound)
	{
		UGameplayStatics::PlaySound2D(this, FallSound);
	}
}
void ABeyondTFRacePlayerState::ClientFinishReached_Implementation()
{
	if (FinishSound)
	{
		UGameplayStatics::PlaySound2D(this, FinishSound);
	}
}
