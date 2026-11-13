#include "BeyondTheFallPlayerControl.h"

#include "BeyondTheFallMainMenuWidget.h"

void ABeyondTheFallPlayerControl::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	MainMenuWidget = CreateWidget<UBeyondTheFallMainMenuWidget>(
		this,
		UBeyondTheFallMainMenuWidget::StaticClass()
	);

	if (!MainMenuWidget)
	{
		return;
	}

	MainMenuWidget->AddToViewport();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());

	SetInputMode(InputMode);
}
