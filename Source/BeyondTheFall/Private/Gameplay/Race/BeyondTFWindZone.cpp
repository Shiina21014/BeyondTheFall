#include "BeyondTFWindZone.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"


ABeyondTFWindZone::ABeyondTFWindZone()

{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WindBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WindBox"));
	RootComponent = WindBox;

	WindBox->SetBoxExtent(FVector(350.0f, 250.0f, 150.0f));
	WindBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WindBox->SetCollisionObjectType(ECC_WorldDynamic);
	WindBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WindBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WindBox->SetGenerateOverlapEvents(true);

	WindDirection = CreateDefaultSubobject<UArrowComponent>(
		TEXT("WindDirection")
	);
	WindDirection->SetupAttachment(WindBox);
	WindDirection->SetArrowSize(3.0f);
	FanPivot = CreateDefaultSubobject<USceneComponent>(TEXT("FanPivot"));
	FanPivot->SetupAttachment(WindBox);
	FanPivot->SetRelativeLocation(FVector(-350.0f, 0.0f, 0.0f));

	FanBlade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FanBlade"));
	FanBlade->SetupAttachment(FanPivot);
	FanBlade->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FanBlade->SetRelativeScale3D(FVector(0.1f, 2.5f, 0.25f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		FanBlade->SetStaticMesh(CubeMesh.Object);
	}

}
void ABeyondTFWindZone::BeginPlay()
{
	Super::BeginPlay();

	if (FanBlade)
	{
		InitialBladeScale = FanBlade->GetRelativeScale3D();
	}
}

void ABeyondTFWindZone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GustEffectRemaining > 0.0f)
	{
		GustEffectRemaining -= DeltaSeconds;

		if (GustEffectRemaining <= 0.0f && FanBlade)
		{
			FanBlade->SetRelativeScale3D(InitialBladeScale);
		}
	}

	if (bWindActive && FanPivot)
	{
		FanPivot->AddLocalRotation(
			FRotator(0.0f, 0.0f, FanRotationSpeed * DeltaSeconds)
		);
	}


	if (!HasAuthority())
	{
		return;
	}

	PhaseTime += DeltaSeconds;

	const float CurrentDuration = bWindActive
		? WindDuration
		: PauseDuration;

	if (PhaseTime >= CurrentDuration)
	{
		PhaseTime = 0.0f;
		bWindActive = !bWindActive;

		if (bWindActive)
		{
			MulticastWindGust();
		}
	}


	if (!bWindActive)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	WindBox->GetOverlappingActors(
		OverlappingActors,
		ACharacter::StaticClass()
	);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		ACharacter* Character = Cast<ACharacter>(OverlappingActor);
		if (!Character)
		{
			continue;
		}

		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

		if (Movement && Movement->MovementMode != MOVE_None)
		{
			const FVector WindForce =
				GetActorForwardVector() * WindAcceleration * Movement->Mass;

			Movement->AddForce(WindForce);
		}
	}
}
void ABeyondTFWindZone::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABeyondTFWindZone, bWindActive);
}
void ABeyondTFWindZone::MulticastWindGust_Implementation()
{
	if (!FanBlade)
	{
		return;
	}

	FanBlade->SetRelativeScale3D(InitialBladeScale * 1.2f);
	GustEffectRemaining = 0.4f;
}

