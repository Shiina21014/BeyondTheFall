#include "BeyondTFFallZone.h"

#include "Gameplay/Race/BeyondTFRaceManager.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"

ABeyondTFFallZone::ABeyondTFFallZone()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(2000.0f, 2000.0f, 100.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);
}

void ABeyondTFFallZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!Cast<ACharacter>(OtherActor))
	{
		return;
	}

	for (TActorIterator<ABeyondTFRaceManager> It(GetWorld()); It; ++It)
	{
		It->RespawnPlayer(OtherActor);
		return;
	}
}

