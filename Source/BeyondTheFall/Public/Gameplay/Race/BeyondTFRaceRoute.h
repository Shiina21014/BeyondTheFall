// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFRaceRoute.generated.h"

class USplineComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFRaceRoute : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFRaceRoute();

	UPROPERTY(VisibleAnywhere, Category = "Race")
	TObjectPtr<USplineComponent> RouteSpline;
	
	UPROPERTY(EditInstanceOnly, Category = "Race")
    int32 RouteId = 1;

};
