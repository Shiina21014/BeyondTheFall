#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFCheckpoint.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFCheckpoint();
	int32 GetCheckpointNumber() const { return CheckpointNumber; }
	int32 GetRouteId() const { return RouteId; }


protected:
	UFUNCTION()
	void OnCheckpointOverlap(
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
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	int32 CheckpointNumber = 1;
	UPROPERTY(EditInstanceOnly, Category = "Checkpoint")
	int32 RouteId = 0;
};
