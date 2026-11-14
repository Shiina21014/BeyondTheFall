#include "BeyondTFRotatingPlatform.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABeyondTFRotatingPlatform::ABeyondTFRotatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 30.0f;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	PlatformMesh->SetMobility(EComponentMobility::Movable);
	PlatformMesh->SetCollisionProfileName(TEXT("BlockAll"));
	PlatformMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.25f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		PlatformMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ABeyondTFRotatingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority())
	{
		return;
	}

	if (bRotating)
	{
		AddActorLocalRotation(
			FRotator(0.0f, RotationSpeed * DeltaSeconds, 0.0f)
		);
	}

	PhaseTime += DeltaSeconds;

	const float CurrentDuration = bRotating
		? RotationDuration
		: PauseDuration;

	if (PhaseTime >= CurrentDuration)
	{
		PhaseTime = 0.0f;
		bRotating = !bRotating;
	}
}
