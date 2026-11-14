#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BeyondTFRaceGM.generated.h"

class ACharacter;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRaceGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABeyondTFRaceGM();

	virtual void Tick(float DeltaSeconds) override;

	void StartCountdown();
	
	void RegisterFinish(ACharacter* Character);
	void CheckRaceEnd();

private:
	
	int32 NextFinishPosition = 1;
	float PositionUpdateTime = 0.0f;

	void UpdateRacePositions();

	
	void AdvanceCountdown();
	void AdvanceRaceClock();

	FTimerHandle CountdownTimer;
	FTimerHandle RaceClockTimer;
};