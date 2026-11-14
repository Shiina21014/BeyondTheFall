#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BeyondTFRacePlayerState.generated.h"

class USoundWave;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRacePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	ABeyondTFRacePlayerState();

	UPROPERTY()
	TObjectPtr<USoundWave> CheckpointSound;
	
	UPROPERTY()
	TObjectPtr<USoundWave> FallSound;
	
	UPROPERTY()
	TObjectPtr<USoundWave> FinishSound;
	
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	UFUNCTION(Client, Reliable)
	void ClientCheckpointReached(int32 ReachedCheckpoint);
	
	UFUNCTION(Client, Reliable)
	void ClientFallOccurred();
	
	UFUNCTION(Client, Reliable)
	void ClientFinishReached();
	
	UFUNCTION(Client, Reliable)
	void ClientAttemptsRestored(int32 RestoredAttempts);

	int32 PitNoticeAttempts = 0;
	float PitNoticeEndTime = 0.0f;
	
	int32 CheckpointNoticeNumber = 0;
	float CheckpointNoticeEndTime = 0.0f;
	
	UFUNCTION(Client, Reliable)
	void ClientDisqualified();

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 CheckpointNumber = 0;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 SelectedRouteId = 0;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 CurrentPosition = 0;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 AttemptsRemaining = 4;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 TotalFalls = 0;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	bool bDisqualified = false;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	bool bFinishedRace = false;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 FinishPosition = 0;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	float FinishTime = 0.0f;

	
	FTransform RespawnTransform;

	bool bHasCheckpoint = false;
};