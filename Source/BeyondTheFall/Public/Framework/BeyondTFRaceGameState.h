#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BeyondTFRaceGameState.generated.h"

UENUM()
enum class ERacePhase : uint8
{
	Waiting,
	Countdown,
	Racing,
	Finished
};

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRaceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABeyondTFRaceGameState();

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	ERacePhase RacePhase = ERacePhase::Waiting;

	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	float RaceTime = 0.0f;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Race")
	int32 CountdownNumber = 0;
};