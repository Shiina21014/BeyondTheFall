#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFRaceManager.generated.h"

class USceneComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRaceManager : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFRaceManager();

	void SetCheckpoint(
	AActor* Player,
	const FTransform& NewCheckpoint,
	int32 CheckpointNumber,
	int32 RouteId
);


	void RespawnPlayer(AActor* PlayerToRespawn);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;

	FTransform InitialRespawnTransform;
};