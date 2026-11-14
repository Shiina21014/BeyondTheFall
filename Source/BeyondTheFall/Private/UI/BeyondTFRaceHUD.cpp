#include "UI/BeyondTFRaceHUD.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Framework/BeyondTFRaceGameState.h"
#include "Framework/BeyondTFRacePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/SWidget.h"
#include "BeyondTFRaceGM.h"
#include "Components/Button.h"
#include "BeyondTheFallCharacter.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"

UBeyondTFRaceHUD::UBeyondTFRaceHUD(
	const FObjectInitializer& ObjectInitializer
)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> CountdownAsset(
		TEXT("/Game/Audio_SFX/Cuenta_regresiva.Cuenta_regresiva")
	);

	if (CountdownAsset.Succeeded())
	{
		CountdownSound = CountdownAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> VictoryAsset(
	TEXT("/Game/Audio_SFX/Victoria.Victoria")
);

	if (VictoryAsset.Succeeded())
	{
		VictorySound = VictoryAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> DefeatAsset(
	TEXT("/Game/Audio_SFX/Derrota.Derrota")
);

	if (DefeatAsset.Succeeded())
	{
		DefeatSound = DefeatAsset.Object;
	}

}

TSharedRef<SWidget> UBeyondTFRaceHUD::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>();
		WidgetTree->RootWidget = Root;

		// Intentos en la esquina superior derecha.
		AttemptsText = WidgetTree->ConstructWidget<UTextBlock>();
		AttemptsText->SetText(FText::FromString(TEXT("Intentos: 4")));

		FSlateFontInfo AttemptsFont = AttemptsText->GetFont();
		AttemptsFont.Size = 24;
		AttemptsText->SetFont(AttemptsFont);

		UCanvasPanelSlot* AttemptsSlot = Root->AddChildToCanvas(AttemptsText);
		AttemptsSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		AttemptsSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		AttemptsSlot->SetPosition(FVector2D(-30.0f, 30.0f));
		AttemptsSlot->SetAutoSize(true);
		PositionText = WidgetTree->ConstructWidget<UTextBlock>();
		PositionText->SetText(FText::FromString(TEXT("Posicion: --")));

		FSlateFontInfo PositionFont = PositionText->GetFont();
		PositionFont.Size = 24;
		PositionText->SetFont(PositionFont);

		UCanvasPanelSlot* PositionSlot = Root->AddChildToCanvas(PositionText);
		PositionSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		PositionSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		PositionSlot->SetPosition(FVector2D(-30.0f, 105.0f));
		PositionSlot->SetAutoSize(true);
		
		CheckpointNoticeText = WidgetTree->ConstructWidget<UTextBlock>();
		CheckpointNoticeText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo CheckpointFont = CheckpointNoticeText->GetFont();
		CheckpointFont.Size = 22;
		CheckpointFont.OutlineSettings.OutlineSize = 2;
		CheckpointFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		CheckpointNoticeText->SetFont(CheckpointFont);

		CheckpointNoticeText->SetColorAndOpacity(
			FSlateColor(FLinearColor(1.0f, 0.85f, 0.0f, 1.0f))
		);

		UCanvasPanelSlot* CheckpointSlot =
			Root->AddChildToCanvas(CheckpointNoticeText);

		CheckpointSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		CheckpointSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		CheckpointSlot->SetPosition(FVector2D(-30.0f, 145.0f));
		CheckpointSlot->SetAutoSize(true);
		PitNoticeText = WidgetTree->ConstructWidget<UTextBlock>();
		PitNoticeText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo PitNoticeFont = PitNoticeText->GetFont();
		PitNoticeFont.Size = 22;
		PitNoticeFont.OutlineSettings.OutlineSize = 2;
		PitNoticeFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		PitNoticeText->SetFont(PitNoticeFont);

		PitNoticeText->SetColorAndOpacity(
			FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f))
		);

		UCanvasPanelSlot* PitNoticeSlot = Root->AddChildToCanvas(PitNoticeText);
		PitNoticeSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		PitNoticeSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		PitNoticeSlot->SetPosition(FVector2D(-30.0f, 180.0f));
		PitNoticeSlot->SetAutoSize(true);



		StaminaBar = WidgetTree->ConstructWidget<UProgressBar>();
		StaminaBar->SetPercent(1.0f);
		StaminaBar->SetFillColorAndOpacity(FLinearColor(0.1f, 0.8f, 0.2f, 1.0f));

		UCanvasPanelSlot* StaminaSlot = Root->AddChildToCanvas(StaminaBar);
		StaminaSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		StaminaSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		StaminaSlot->SetPosition(FVector2D(-30.0f, 70.0f));
		StaminaSlot->SetSize(FVector2D(200.0f, 20.0f));
		UTextBlock* StaminaLabel = WidgetTree->ConstructWidget<UTextBlock>();
		StaminaLabel->SetText(FText::FromString(TEXT("STAMINA")));

		FSlateFontInfo StaminaFont = StaminaLabel->GetFont();
		StaminaFont.Size = 18;
		StaminaLabel->SetFont(StaminaFont);

		UCanvasPanelSlot* LabelSlot = Root->AddChildToCanvas(StaminaLabel);
		LabelSlot->SetAnchors(FAnchors(1.0f, 0.0f));
		LabelSlot->SetAlignment(FVector2D(1.0f, 0.0f));
		LabelSlot->SetPosition(FVector2D(-240.0f, 68.0f));
		LabelSlot->SetAutoSize(true);
		
		CountdownText = WidgetTree->ConstructWidget<UTextBlock>();
		CountdownText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo CountdownFont = CountdownText->GetFont();
		CountdownFont.Size = 72;
		CountdownFont.OutlineSettings.OutlineSize = 2;
		CountdownFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		CountdownText->SetFont(CountdownFont);

		CountdownText->SetColorAndOpacity(
			FSlateColor(FLinearColor(0.05f, 1.0f, 0.1f, 1.0f))
		);
		UCanvasPanelSlot* CountdownSlot = Root->AddChildToCanvas(CountdownText);
		CountdownSlot->SetAnchors(FAnchors(0.5f, 0.0f));
		CountdownSlot->SetAlignment(FVector2D(0.5f, 0.0f));
		CountdownSlot->SetPosition(FVector2D(0.0f, 60.0f));
		CountdownSlot->SetAutoSize(true);
		
		WaitingText = WidgetTree->ConstructWidget<UTextBlock>();
		WaitingText->SetText(FText::FromString(TEXT("ESPERANDO AL HOST")));
		WaitingText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo WaitingFont = WaitingText->GetFont();
		WaitingFont.Size = 32;
		WaitingFont.OutlineSettings.OutlineSize = 2;
		WaitingFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		WaitingText->SetFont(WaitingFont);

		WaitingText->SetColorAndOpacity(
			FSlateColor(FLinearColor(1.0f, 0.05f, 0.05f, 1.0f))
		);

		UCanvasPanelSlot* WaitingSlot = Root->AddChildToCanvas(WaitingText);
		WaitingSlot->SetAnchors(FAnchors(0.5f, 0.4f));
		WaitingSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		WaitingSlot->SetPosition(FVector2D::ZeroVector);
		WaitingSlot->SetAutoSize(true);
		ResultsText = WidgetTree->ConstructWidget<UTextBlock>();
		ResultsText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo ResultsFont = ResultsText->GetFont();
		ResultsFont.Size = 24;
		ResultsFont.OutlineSettings.OutlineSize = 2;
		ResultsFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		ResultsText->SetFont(ResultsFont);

		UCanvasPanelSlot* ResultsSlot = Root->AddChildToCanvas(ResultsText);
		ResultsSlot->SetAnchors(FAnchors(0.0f, 1.0f));
		ResultsSlot->SetAlignment(FVector2D(0.0f, 1.0f));
		ResultsSlot->SetPosition(FVector2D(30.0f, -30.0f));
		ResultsSlot->SetAutoSize(true);
		
		DisqualifiedText = WidgetTree->ConstructWidget<UTextBlock>();
		DisqualifiedText->SetText(FText::FromString(TEXT("DESCALIFICADO")));
		DisqualifiedText->SetVisibility(ESlateVisibility::Hidden);

		FSlateFontInfo DisqualifiedFont = DisqualifiedText->GetFont();
		DisqualifiedFont.Size = 48;
		DisqualifiedFont.OutlineSettings.OutlineSize = 2;
		DisqualifiedFont.OutlineSettings.OutlineColor = FLinearColor::Black;
		DisqualifiedText->SetFont(DisqualifiedFont);

		DisqualifiedText->SetColorAndOpacity(
			FSlateColor(FLinearColor(1.0f, 0.05f, 0.05f, 1.0f))
		);

		UCanvasPanelSlot* DisqualifiedSlot =
			Root->AddChildToCanvas(DisqualifiedText);

		DisqualifiedSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		DisqualifiedSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		DisqualifiedSlot->SetPosition(FVector2D::ZeroVector);
		DisqualifiedSlot->SetAutoSize(true);

		
		RestartButton = WidgetTree->ConstructWidget<UButton>();
		RestartButton->SetVisibility(ESlateVisibility::Hidden);

		UTextBlock* RestartText = WidgetTree->ConstructWidget<UTextBlock>();
		RestartText->SetText(FText::FromString(TEXT("VOLVER A JUGAR")));
		RestartButton->SetContent(RestartText);
		RestartButton->OnClicked.AddDynamic(this, &UBeyondTFRaceHUD::OnRestartClicked);

		UCanvasPanelSlot* RestartSlot = Root->AddChildToCanvas(RestartButton);
		RestartSlot->SetAnchors(FAnchors(0.5f, 0.78f));
		RestartSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		RestartSlot->SetPosition(FVector2D::ZeroVector);
		RestartSlot->SetSize(FVector2D(300.0f, 55.0f));

		ExitButton = WidgetTree->ConstructWidget<UButton>();
		ExitButton->SetVisibility(ESlateVisibility::Hidden);

		UTextBlock* ExitText = WidgetTree->ConstructWidget<UTextBlock>();
		ExitText->SetText(FText::FromString(TEXT("SALIR AL MENU")));
		ExitButton->SetContent(ExitText);
		ExitButton->OnClicked.AddDynamic(this, &UBeyondTFRaceHUD::OnExitClicked);

		UCanvasPanelSlot* ExitSlot = Root->AddChildToCanvas(ExitButton);
		ExitSlot->SetAnchors(FAnchors(0.5f, 0.88f));
		ExitSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		ExitSlot->SetPosition(FVector2D::ZeroVector);
		ExitSlot->SetSize(FVector2D(300.0f, 55.0f));


		
		StartButton = WidgetTree->ConstructWidget<UButton>();

		UTextBlock* StartText = WidgetTree->ConstructWidget<UTextBlock>();
		StartText->SetText(FText::FromString(TEXT("INICIAR CARRERA")));

		StartButton->SetContent(StartText);
		StartButton->OnClicked.AddDynamic(this, &UBeyondTFRaceHUD::OnStartClicked);

		UCanvasPanelSlot* StartSlot = Root->AddChildToCanvas(StartButton);
		StartSlot->SetAnchors(FAnchors(0.5f, 0.75f));
		StartSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		StartSlot->SetPosition(FVector2D::ZeroVector);
		StartSlot->SetSize(FVector2D(380.0f, 70.0f));
	}

	return Super::RebuildWidget();
}

void UBeyondTFRaceHUD::NativeTick(
	const FGeometry& MyGeometry,
	float InDeltaTime
)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}
	
	ABeyondTheFallCharacter* Character =
	Cast<ABeyondTheFallCharacter>(PlayerController->GetPawn());

	if (StaminaBar && Character && Character->MaxStamina > 0.0f)
	{
		const float StaminaPercent =
			Character->CurrentStamina / Character->MaxStamina;

		StaminaBar->SetPercent(FMath::Clamp(StaminaPercent, 0.0f, 1.0f));
	}


	ABeyondTFRacePlayerState* LocalRacePlayerState =
	PlayerController->GetPlayerState<ABeyondTFRacePlayerState>();

	if (AttemptsText && LocalRacePlayerState)
	{
		AttemptsText->SetText(FText::FromString(
			FString::Printf(
				TEXT("Intentos: %d"),
				LocalRacePlayerState->AttemptsRemaining
			)
		));
	}


	ABeyondTFRaceGameState* RaceGameState =
		GetWorld()->GetGameState<ABeyondTFRaceGameState>();

	if (!CountdownText || !RaceGameState)
	{
		return;
	}
	if (
	!bDefeatSoundPlayed
	&& RaceGameState->RacePhase == ERacePhase::Finished
	&& LocalRacePlayerState
	&& (
		LocalRacePlayerState->bDisqualified
		|| (
			LocalRacePlayerState->bFinishedRace
			&& LocalRacePlayerState->FinishPosition > 1
		)
	)
)
	{
		bDefeatSoundPlayed = true;

		if (DefeatSound)
		{
			UGameplayStatics::PlaySound2D(this, DefeatSound);
		}
	}

	if (
	!bVictorySoundPlayed
	&& RaceGameState->RacePhase == ERacePhase::Finished
	&& LocalRacePlayerState
	&& LocalRacePlayerState->bFinishedRace
	&& LocalRacePlayerState->FinishPosition == 1
)
	{
		bVictorySoundPlayed = true;

		if (VictorySound)
		{
			UGameplayStatics::PlaySound2D(this, VictorySound);
		}
	}

	if (RaceGameState->RacePhase == ERacePhase::Countdown && !bCountdownSoundPlayed)
	{
		bCountdownSoundPlayed = true;

		if (CountdownSound)
		{
			UGameplayStatics::PlaySound2D(this, CountdownSound);
		}
	}
	else if (RaceGameState->RacePhase == ERacePhase::Waiting)
	{
		bCountdownSoundPlayed = false;
	}

	
	if (PitNoticeText)
	{
		const bool bShowPitNotice =
			LocalRacePlayerState
			&& GetWorld()
			&& GetWorld()->GetTimeSeconds() < LocalRacePlayerState->PitNoticeEndTime
			&& RaceGameState->RacePhase != ERacePhase::Finished;

		if (bShowPitNotice)
		{
			const int32 Amount = LocalRacePlayerState->PitNoticeAttempts;

			const FString Message = Amount == 1
				? TEXT("Recuperaste 1 intento")
				: FString::Printf(TEXT("Recuperaste %d intentos"), Amount);

			PitNoticeText->SetText(FText::FromString(Message));
			PitNoticeText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PitNoticeText->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	
	if (CheckpointNoticeText)
	{
		const bool bShowCheckpoint =
			LocalRacePlayerState
			&& GetWorld()
			&& GetWorld()->GetTimeSeconds() < LocalRacePlayerState->CheckpointNoticeEndTime
			&& RaceGameState->RacePhase != ERacePhase::Finished;

		if (bShowCheckpoint)
		{
			CheckpointNoticeText->SetText(FText::FromString(
				FString::Printf(
					TEXT("Checkpoint %d alcanzado"),
					LocalRacePlayerState->CheckpointNoticeNumber
				)
			));

			CheckpointNoticeText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CheckpointNoticeText->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	
	if (DisqualifiedText)
	{
		const bool bShowDisqualified =
			LocalRacePlayerState
			&& LocalRacePlayerState->bDisqualified
			&& RaceGameState->RacePhase != ERacePhase::Finished;

		DisqualifiedText->SetVisibility(
			bShowDisqualified ? ESlateVisibility::Visible : ESlateVisibility::Hidden
		);
	}

	
	if (PositionText && LocalRacePlayerState)
	{
		const int32 Position = LocalRacePlayerState->CurrentPosition;
		const int32 TotalPlayers = RaceGameState->PlayerArray.Num();

		if (Position > 0)
		{
			PositionText->SetText(FText::FromString(
				FString::Printf(TEXT("Posicion: %d/%d"), Position, TotalPlayers)
			));
		}
		else
		{
			PositionText->SetText(FText::FromString(TEXT("Posicion: --")));
		}
	}

	
	const bool bRaceFinished = RaceGameState->RacePhase == ERacePhase::Finished;

	if (RestartButton)
	{
		RestartButton->SetVisibility(
			bRaceFinished && PlayerController->HasAuthority()
				? ESlateVisibility::Visible
				: ESlateVisibility::Hidden
		);
	}

	if (ExitButton)
	{
		ExitButton->SetVisibility(
			bRaceFinished ? ESlateVisibility::Visible : ESlateVisibility::Hidden
		);
	}

	if (bRaceFinished && !bResultsInputConfigured)
	{
		bResultsInputConfigured = true;
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}

	
	if (ResultsText)
	{
		if (RaceGameState->RacePhase == ERacePhase::Finished)
		{
			FString Results = TEXT("RESULTADOS\n\n");
			bool bHasWinner = false;

			const int32 PlayerCount = RaceGameState->PlayerArray.Num();

			for (int32 Position = 1; Position <= PlayerCount; Position++)
			{
				for (APlayerState* PlayerState : RaceGameState->PlayerArray)
				{
					ABeyondTFRacePlayerState* RacePlayerState =
						Cast<ABeyondTFRacePlayerState>(PlayerState);

					if (!RacePlayerState || RacePlayerState->FinishPosition != Position)
					{
						continue;
					}

					if (Position == 1)
					{
						Results += FString::Printf(
							TEXT("Ganador: %s\n\n"),
							*RacePlayerState->GetPlayerName()
						);
						bHasWinner = true;
					}

					const int32 TotalSeconds =
						FMath::FloorToInt(RacePlayerState->FinishTime);

					Results += FString::Printf(
						TEXT("%d. %s | Caidas: %d | Tiempo: %02d:%02d\n"),
						Position,
						*RacePlayerState->GetPlayerName(),
						4 - RacePlayerState->TotalFalls,
						TotalSeconds / 60,
						TotalSeconds % 60
					);
				}
			}

			if (!bHasWinner)
			{
				Results += TEXT("Sin ganador\n");
			}

			for (APlayerState* PlayerState : RaceGameState->PlayerArray)
			{
				ABeyondTFRacePlayerState* RacePlayerState =
					Cast<ABeyondTFRacePlayerState>(PlayerState);

				if (!RacePlayerState || !RacePlayerState->bDisqualified)
				{
					continue;
				}

				const int32 TotalSeconds =
					FMath::FloorToInt(RacePlayerState->FinishTime);

				Results += FString::Printf(
					TEXT("%s | DESCALIFICADO | Caidas: %d | Tiempo: %02d:%02d\n"),
					*RacePlayerState->GetPlayerName(),
					4 - RacePlayerState->TotalFalls,
					TotalSeconds / 60,
					TotalSeconds % 60
				);
			}

			ResultsText->SetText(FText::FromString(Results));
			ResultsText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ResultsText->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WaitingText)
	{
		const bool bWaitingForHost =
			!PlayerController->HasAuthority()
			&& RaceGameState->RacePhase == ERacePhase::Waiting;

		WaitingText->SetVisibility(
			bWaitingForHost ? ESlateVisibility::Visible : ESlateVisibility::Hidden
		);
	}
	if (StartButton)
	{
		const bool bCanStart =
			PlayerController->HasAuthority()
			&& RaceGameState->RacePhase == ERacePhase::Waiting;

		StartButton->SetVisibility(
			bCanStart ? ESlateVisibility::Visible : ESlateVisibility::Hidden
		);
	}
	
	if (RaceGameState->RacePhase == ERacePhase::Countdown)
	{
		CountdownText->SetVisibility(ESlateVisibility::Visible);

		if (RaceGameState->CountdownNumber > 0)
		{
			CountdownText->SetText(FText::AsNumber(
				RaceGameState->CountdownNumber
			));
		}
		else
		{
			CountdownText->SetText(FText::FromString(TEXT("¡YA!")));
		}
	}
	else if (RaceGameState->RacePhase == ERacePhase::Racing)
	{
		const int32 TotalSeconds = FMath::FloorToInt(RaceGameState->RaceTime);
		const int32 Minutes = TotalSeconds / 60;
		const int32 Seconds = TotalSeconds % 60;

		CountdownText->SetVisibility(ESlateVisibility::Visible);
		CountdownText->SetText(FText::FromString(
			FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)
		));
	}
	else
	{
		CountdownText->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UBeyondTFRaceHUD::OnStartClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController || !PlayerController->HasAuthority())
	{
		return;
	}

	ABeyondTFRaceGM* RaceGameMode =
		GetWorld()->GetAuthGameMode<ABeyondTFRaceGM>();

	if (RaceGameMode)
	{
		RaceGameMode->StartCountdown();

		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void UBeyondTFRaceHUD::OnRestartClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController || !PlayerController->HasAuthority())
	{
		return;
	}

	ABeyondTFRaceGameState* RaceGameState =
		GetWorld()->GetGameState<ABeyondTFRaceGameState>();

	if (!RaceGameState || RaceGameState->RacePhase != ERacePhase::Finished)
	{
		return;
	}

	GetWorld()->ServerTravel(TEXT("/Game/Mapas/L_RaceMap?listen"));
}

void UBeyondTFRaceHUD::OnExitClicked()
{
	UGameplayStatics::OpenLevel(
		this,
		FName(TEXT("/Game/Mapas/L_MainMenuMap"))
	);
}
