#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BeyondTheFallMainMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class BEYONDTHEFALL_API UBeyondTheFallMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<UButton> StartButton;

	UPROPERTY()
	TObjectPtr<UButton> ExitButton;

	UFUNCTION()
	void HandleStartClicked();

	UFUNCTION()
	void HandleExitClicked();
};