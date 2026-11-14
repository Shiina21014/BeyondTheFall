#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFPendulum.generated.h"

class ACharacter;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFPendulum : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFPendulum();

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Pendulum")
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, Category = "Pendulum")
	TObjectPtr<UStaticMeshComponent> PendulumHead;

	UPROPERTY(EditInstanceOnly, Category = "Pendulum", meta = (ClampMin = "0.0"))
	float SwingAngle = 50.0f;

	UPROPERTY(EditInstanceOnly, Category = "Pendulum", meta = (ClampMin = "0.0"))
	float SwingSpeed = 2.0f;

	UPROPERTY(EditInstanceOnly, Category = "Pendulum", meta = (ClampMin = "0.0"))
	float PushStrength = 900.0f;

	UPROPERTY(EditInstanceOnly, Category = "Pendulum", meta = (ClampMin = "0.0"))
	float PushUpward = 250.0f;
	
	UPROPERTY(EditInstanceOnly, Category = "Pendulum")
	float InitialPhaseDegrees = 0.0f;
	
	float SwingTime = 0.0f;

	TMap<TWeakObjectPtr<ACharacter>, float> LastPushTimes;
};
