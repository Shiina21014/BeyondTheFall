#include "BeyondTFPendulum.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

ABeyondTFPendulum::ABeyondTFPendulum()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 30.0f;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	RootComponent = Pivot;

	PendulumHead = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("PendulumHead")
	);
	PendulumHead->SetupAttachment(Pivot);
	PendulumHead->SetMobility(EComponentMobility::Movable);
	PendulumHead->SetRelativeLocation(FVector(0.0f, 0.0f, -300.0f));
	PendulumHead->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	PendulumHead->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PendulumHead->SetCollisionResponseToAllChannels(ECR_Ignore);
	PendulumHead->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PendulumHead->SetGenerateOverlapEvents(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		PendulumHead->SetStaticMesh(CubeMesh.Object);
	}
}

void ABeyondTFPendulum::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority())
	{
		return;
	}

	SwingTime += DeltaSeconds;

	const float Angle = FMath::Sin(
	SwingTime * SwingSpeed + FMath::DegreesToRadians(InitialPhaseDegrees)
) * SwingAngle;
	FRotator NewRotation = GetActorRotation();
	NewRotation.Roll = Angle;
	SetActorRotation(NewRotation);



	TArray<AActor*> OverlappingActors;
	PendulumHead->GetOverlappingActors(
		OverlappingActors,
		ACharacter::StaticClass()
	);

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	for (AActor* OverlappingActor : OverlappingActors)
	{
		ACharacter* Character = Cast<ACharacter>(OverlappingActor);
		if (!Character)
		{
			continue;
		}

		const TWeakObjectPtr<ACharacter> CharacterKey(Character);
		const float* LastPushTime = LastPushTimes.Find(CharacterKey);

		if (LastPushTime && CurrentTime - *LastPushTime < 1.0f)
		{
			continue;
		}

		FVector PushDirection =
			Character->GetActorLocation() - PendulumHead->GetComponentLocation();

		PushDirection.Z = 0.0f;
		PushDirection = PushDirection.GetSafeNormal();

		if (PushDirection.IsNearlyZero())
		{
			PushDirection = GetActorForwardVector();
			PushDirection.Z = 0.0f;
			PushDirection.Normalize();
		}

		Character->LaunchCharacter(
			PushDirection * PushStrength + FVector(0.0f, 0.0f, PushUpward),
			true,
			true
		);

		LastPushTimes.Add(CharacterKey, CurrentTime);
	}
}
