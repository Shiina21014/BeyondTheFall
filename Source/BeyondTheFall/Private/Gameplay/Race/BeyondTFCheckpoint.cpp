#include "Gameplay/Race/BeyondTFCheckpoint.h"

#include "BeyondTFRaceManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

ABeyondTFCheckpoint::ABeyondTFCheckpoint()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(150.0f, 150.0f, 200.0f));
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
	CollisionBox->SetGenerateOverlapEvents(true);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ABeyondTFCheckpoint::OnCheckpointOverlap
	);

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("CheckpointMesh")
	);
	CheckpointMesh->SetupAttachment(CollisionBox);
	CheckpointMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	CheckpointMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 3.0f));
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (CubeMesh.Succeeded())
	{
		CheckpointMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ABeyondTFCheckpoint::OnCheckpointOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!HasAuthority() || !Cast<ACharacter>(OtherActor))
	{
		return;
	}

	for (TActorIterator<ABeyondTFRaceManager> It(GetWorld()); It; ++It)
	{
		It->SetCheckpoint(OtherActor, GetActorTransform(), CheckpointNumber, RouteId);
	}
}