#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFWindZone.generated.h"

class UArrowComponent;
class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFWindZone : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFWindZone();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Wind")
	TObjectPtr<UBoxComponent> WindBox;

	UPROPERTY(VisibleAnywhere, Category = "Wind")
	TObjectPtr<UArrowComponent> WindDirection;
	
	UPROPERTY(VisibleAnywhere, Category = "Wind")
	TObjectPtr<USceneComponent> FanPivot;

	UPROPERTY(VisibleAnywhere, Category = "Wind")
	TObjectPtr<UStaticMeshComponent> FanBlade;

	UPROPERTY(EditInstanceOnly, Category = "Wind", meta = (ClampMin = "0.0"))
	float FanRotationSpeed = 720.0f;
	
	UPROPERTY(EditInstanceOnly, Category = "Wind", meta = (ClampMin = "0.0"))
	float WindAcceleration = 900.0f;
	
	UPROPERTY(EditInstanceOnly, Category = "Wind", meta = (ClampMin = "0.1"))
	float WindDuration = 3.0f;

	UPROPERTY(EditInstanceOnly, Category = "Wind", meta = (ClampMin = "0.1"))
	float PauseDuration = 2.0f;

	float PhaseTime = 0.0f;
	UPROPERTY(Replicated)
	bool bWindActive = true;
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastWindGust();

	FVector InitialBladeScale = FVector::OneVector;
	float GustEffectRemaining = 0.0f;

};

