#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeyondTFFallZone.generated.h"

class UBoxComponent;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFFallZone : public AActor
{
	GENERATED_BODY()

public:
	ABeyondTFFallZone();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;
};
