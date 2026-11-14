// Fill out your copyright notice in the Description page of Project Settings.

#include "BeyondTFRaceRoute.h"

#include "Components/SplineComponent.h"

ABeyondTFRaceRoute::ABeyondTFRaceRoute()
{
	RouteSpline = CreateDefaultSubobject<USplineComponent>(TEXT("RouteSpline"));
	RootComponent = RouteSpline;

	RouteSpline->SetClosedLoop(false);
}

