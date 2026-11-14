// Copyright Epic Games, Inc. All Rights Reserved.


#include "BeyondTheFallPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "BeyondTheFall.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "UI/BeyondTFRaceHUD.h"

void ABeyondTheFallPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		UBeyondTFRaceHUD* RaceHUD = CreateWidget<UBeyondTFRaceHUD>(
			this,
			UBeyondTFRaceHUD::StaticClass()
		);

		if (RaceHUD)
		{
			RaceHUD->AddToPlayerScreen();
			if (HasAuthority())
			{
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
			}
		}
	}

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogBeyondTheFall, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ABeyondTheFallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool ABeyondTheFallPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
