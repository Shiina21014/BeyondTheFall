#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFFinish.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFFinish : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFFinish();

protected:
	UFUNCTION()
	void OnFinishOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> FinishMesh;

	bool bFinished = false;
};
