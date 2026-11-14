#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFRotatingPlatform.generated.h"

class UStaticMeshComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRotatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFRotatingPlatform();

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Platform")
	TObjectPtr<UStaticMeshComponent> PlatformMesh;

	// Grados por segundo.
	UPROPERTY(EditInstanceOnly, Category = "Platform", meta = (ClampMin = "0.0"))
	float RotationSpeed = 120.0f;

	UPROPERTY(EditInstanceOnly, Category = "Platform", meta = (ClampMin = "0.1"))
	float RotationDuration = 3.0f;

	UPROPERTY(EditInstanceOnly, Category = "Platform", meta = (ClampMin = "0.1"))
	float PauseDuration = 1.5f;

	float PhaseTime = 0.0f;
	bool bRotating = true;
};
