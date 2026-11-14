// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFPitStop.generated.h"

class ABeyondTFRacePlayerState;
class UBoxComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFPitStop : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFPitStop();

protected:
	UFUNCTION()
	void OnPitOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY(VisibleAnywhere, Category = "Pit Stop")
	TObjectPtr<UBoxComponent> DetectionBox;

	UPROPERTY(VisibleAnywhere, Category = "Pit Stop")
	TObjectPtr<UStaticMeshComponent> PlatformMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Pit Stop")
	TObjectPtr<UTextRenderComponent> PitSign;

	UPROPERTY(EditInstanceOnly, Category = "Pit Stop",
		meta = (ClampMin = "1", ClampMax = "2"))
	int32 AttemptsToRestore = 1;

	TSet<TWeakObjectPtr<ABeyondTFRacePlayerState>> PlayersWhoUsedPit;
};

