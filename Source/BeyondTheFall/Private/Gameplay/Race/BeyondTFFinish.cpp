#include "BeyondTFFinish.h"

#include "BeyondTFRaceGM.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

ABeyondTFFinish::ABeyondTFFinish()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(150.0f, 300.0f, 250.0f));
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
	CollisionBox->SetGenerateOverlapEvents(true);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ABeyondTFFinish::OnFinishOverlap
	);

	FinishMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("FinishMesh")
	);
	FinishMesh->SetupAttachment(CollisionBox);
	FinishMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	FinishMesh->SetRelativeScale3D(FVector(0.5f, 4.0f, 4.0f));
	FinishMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		FinishMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ABeyondTFFinish::OnFinishOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!HasAuthority())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	ABeyondTFRaceGM* RaceGameMode =
		GetWorld()->GetAuthGameMode<ABeyondTFRaceGM>();

	if (RaceGameMode)
	{
		RaceGameMode->RegisterFinish(Character);
	}
}
