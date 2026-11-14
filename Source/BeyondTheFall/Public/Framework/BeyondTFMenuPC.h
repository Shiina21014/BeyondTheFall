#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BeyondTFMenuPC.generated.h"

class UBeyondTFMenuWidget;

UCLASS()
class BEYONDTHEFALL_API ABeyondTFMenuPC : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBeyondTFMenuWidget> MenuWidget;
};
