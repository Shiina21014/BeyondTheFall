
#include "Framework/BeyondTFMenuPC.h"
#include "UI/BeyondTFMenuWidget.h"


void ABeyondTFMenuPC::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	MenuWidget = CreateWidget<UBeyondTFMenuWidget>(
		this,
		UBeyondTFMenuWidget::StaticClass()
	);

	if (!MenuWidget)
	{
		return;
	}

	MenuWidget->AddToViewport();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());

	SetInputMode(InputMode);
}
