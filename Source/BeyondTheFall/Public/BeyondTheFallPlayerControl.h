#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BeyondTheFallPlayerControl.generated.h"

class UBeyondTheFallMainMenuWidget;

UCLASS()
class BEYONDTHEFALL_API ABeyondTheFallPlayerControl : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBeyondTheFallMainMenuWidget> MainMenuWidget;
};