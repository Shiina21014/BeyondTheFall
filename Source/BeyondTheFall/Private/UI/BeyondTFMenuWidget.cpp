#include "UI/BeyondTFMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/SWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Engine/Font.h"


UBeyondTFMenuWidget::UBeyondTFMenuWidget(
	const FObjectInitializer& ObjectInitializer
)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> MusicAsset(
		TEXT("/Game/Audio_SFX/Menu_principal.Menu_principal")
	);

	if (MusicAsset.Succeeded())
	{
		MenuMusic = MusicAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundWave> ButtonAsset(
	TEXT("/Game/Audio_SFX/Sonido_boton.Sonido_boton")
);

	if (ButtonAsset.Succeeded())
	{
		ButtonSound = ButtonAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BackgroundAsset(
	TEXT("/Game/UI/Portada.Portada")
);

	if (BackgroundAsset.Succeeded())
	{
		MenuBackground = BackgroundAsset.Object;
	}

}

TSharedRef<SWidget> UBeyondTFMenuWidget::RebuildWidget()
{
	BuildMenu();
	return Super::RebuildWidget();
}

void UBeyondTFMenuWidget::BuildMenu()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>();
	WidgetTree->RootWidget = Root;
	UImage* BackgroundImage = WidgetTree->ConstructWidget<UImage>();
	BackgroundImage->SetBrushFromTexture(MenuBackground);
	BackgroundImage->SetVisibility(ESlateVisibility::HitTestInvisible);

	UCanvasPanelSlot* BackgroundSlot = Root->AddChildToCanvas(BackgroundImage);
	BackgroundSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	BackgroundSlot->SetOffsets(FMargin(0.0f));
	BackgroundSlot->SetZOrder(-1);
	
	PlayButton = WidgetTree->ConstructWidget<UButton>();

	UTextBlock* PlayText = WidgetTree->ConstructWidget<UTextBlock>();
	PlayText->SetText(FText::FromString(TEXT("CREAR SALA")));
	PlayText->SetColorAndOpacity(FSlateColor(FLinearColor(0.05f, 0.45f, 1.0f, 1.0f)));
	PlayButton->SetContent(PlayText);
	PlayButton->OnClicked.AddDynamic(this, &UBeyondTFMenuWidget::OnPlayClicked);

	UCanvasPanelSlot* PlaySlot = Root->AddChildToCanvas(PlayButton);
	PlaySlot->SetAnchors(FAnchors(1.0f, 1.0f));
	PlaySlot->SetAlignment(FVector2D(1.0f, 1.0f));
	PlaySlot->SetPosition(FVector2D(-40.0f, -200.0f));
	PlaySlot->SetSize(FVector2D(280.0f, 55.0f));

	ExitButton = WidgetTree->ConstructWidget<UButton>();
	JoinButton = WidgetTree->ConstructWidget<UButton>();

	UTextBlock* JoinText = WidgetTree->ConstructWidget<UTextBlock>();
	JoinText->SetText(FText::FromString(TEXT("UNIRSE A SALA")));
	UFont* TestFont = LoadObject<UFont>(
	nullptr,
	TEXT("/Engine/EngineFonts/DroidSansMono.DroidSansMono")
);

	if (TestFont)
	{
		JoinText->SetFont(FSlateFontInfo(TestFont, 22));
	}

	JoinButton->SetContent(JoinText);
	JoinButton->OnClicked.AddDynamic(this, &UBeyondTFMenuWidget::OnJoinClicked);

	UCanvasPanelSlot* JoinSlot = Root->AddChildToCanvas(JoinButton);
	JoinSlot->SetAnchors(FAnchors(1.0f, 1.0f));
	JoinSlot->SetAlignment(FVector2D(1.0f, 1.0f));
	JoinSlot->SetPosition(FVector2D(-40.0f, -130.0f));
	JoinSlot->SetSize(FVector2D(280.0f, 55.0f));

	IpAddressInput = WidgetTree->ConstructWidget<UEditableTextBox>();
	IpAddressInput->SetHintText(FText::FromString(TEXT("IP del host")));
	const FLinearColor IpTextColor(0.65f, 0.0f, 0.0f, 1.0f);

	FEditableTextBoxStyle IpStyle = IpAddressInput->GetWidgetStyle();
	IpStyle.SetForegroundColor(FSlateColor(IpTextColor));
	IpStyle.SetFocusedForegroundColor(FSlateColor(IpTextColor));
	IpStyle.TextStyle.SetColorAndOpacity(FSlateColor(IpTextColor));

	IpAddressInput->SetWidgetStyle(IpStyle);
	IpAddressInput->SetForegroundColor(IpTextColor);
	
	UCanvasPanelSlot* IpSlot = Root->AddChildToCanvas(IpAddressInput);
	IpSlot->SetAnchors(FAnchors(1.0f, 1.0f));
	IpSlot->SetAlignment(FVector2D(1.0f, 1.0f));
	IpSlot->SetPosition(FVector2D(-340.0f, -130.0f));
	IpSlot->SetSize(FVector2D(240.0f, 55.0f));

	JoinErrorText = WidgetTree->ConstructWidget<UTextBlock>();
	JoinErrorText->SetVisibility(ESlateVisibility::Hidden);
	JoinErrorText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

	UCanvasPanelSlot* ErrorSlot = Root->AddChildToCanvas(JoinErrorText);
	ErrorSlot->SetAnchors(FAnchors(0.5f, 1.0f));
	ErrorSlot->SetAlignment(FVector2D(0.5f, 1.0f));
	ErrorSlot->SetPosition(FVector2D(0.0f, -20.0f));
	ErrorSlot->SetAutoSize(true);

	UTextBlock* ExitText = WidgetTree->ConstructWidget<UTextBlock>();
	ExitText->SetText(FText::FromString(TEXT("SALIR")));
	ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(0.9f, 0.1f, 0.1f, 1.0f)));
	ExitButton->SetContent(ExitText);
	ExitButton->OnClicked.AddDynamic(this, &UBeyondTFMenuWidget::OnExitClicked);

	UCanvasPanelSlot* ExitSlot = Root->AddChildToCanvas(ExitButton);
	ExitSlot->SetAnchors(FAnchors(1.0f, 1.0f));
	ExitSlot->SetAlignment(FVector2D(1.0f, 1.0f));
	ExitSlot->SetPosition(FVector2D(-40.0f, -60.0f));
	ExitSlot->SetSize(FVector2D(280.0f, 55.0f));
}

void UBeyondTFMenuWidget::OnPlayClicked()
{
	QueueMenuAction(1);
}

void UBeyondTFMenuWidget::OnExitClicked()
{
	QueueMenuAction(3);
}
void UBeyondTFMenuWidget::OnJoinClicked()
{
	if (!IpAddressInput || !JoinErrorText)
	{
		return;
	}

	const FString Address = IpAddressInput->GetText().ToString().TrimStartAndEnd();

	TArray<FString> Parts;
	Address.ParseIntoArray(Parts, TEXT("."), false);

	bool bValidAddress = Parts.Num() == 4;

	if (bValidAddress)
	{
		for (const FString& Part : Parts)
		{
			if (Part.IsEmpty() || Part.Len() > 3)
			{
				bValidAddress = false;
				break;
			}

			for (const TCHAR Character : Part)
			{
				if (!FChar::IsDigit(Character))
				{
					bValidAddress = false;
					break;
				}
			}

			if (!bValidAddress || FCString::Atoi(*Part) > 255)
			{
				bValidAddress = false;
				break;
			}
		}
	}

	if (!bValidAddress)
	{
		if (ButtonSound)
		{
			UGameplayStatics::PlaySound2D(this, ButtonSound);
		}

		JoinErrorText->SetText(
			FText::FromString(TEXT("Ingresa una IP valida, por ejemplo 192.168.1.10"))
		);
		JoinErrorText->SetVisibility(ESlateVisibility::Visible);
		return;
	}


	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		return;
	}

	JoinErrorText->SetVisibility(ESlateVisibility::Hidden);
	PendingAddress = Address;
	QueueMenuAction(2);

}
void UBeyondTFMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuMusic && !MenuMusicComponent)
	{
		MenuMusicComponent = UGameplayStatics::SpawnSound2D(
			this,
			MenuMusic,
			1.0f,
			1.0f,
			0.0f,
			nullptr,
			false,
			true
		);
	}
}

void UBeyondTFMenuWidget::NativeDestruct()
{
	if (MenuMusicComponent)
	{
		MenuMusicComponent->Stop();
		MenuMusicComponent = nullptr;
	}

	Super::NativeDestruct();
}
void UBeyondTFMenuWidget::QueueMenuAction(int32 Action)
{
	if (bMenuActionPending || !GetWorld())
	{
		return;
	}

	bMenuActionPending = true;

	if (PlayButton)
	{
		PlayButton->SetIsEnabled(false);
	}

	if (JoinButton)
	{
		JoinButton->SetIsEnabled(false);
	}

	if (ExitButton)
	{
		ExitButton->SetIsEnabled(false);
	}

	float Delay = 0.1f;

	if (ButtonSound)
	{
		UGameplayStatics::PlaySound2D(this, ButtonSound);
		Delay = FMath::Max(ButtonSound->GetDuration(), 0.1f);
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(
		this,
		&UBeyondTFMenuWidget::ExecuteMenuAction,
		Action
	);

	GetWorld()->GetTimerManager().SetTimer(
		MenuActionTimer,
		TimerDelegate,
		Delay,
		false
	);
}

void UBeyondTFMenuWidget::ExecuteMenuAction(int32 Action)
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (Action == 1)
	{
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}

		UGameplayStatics::OpenLevel(
			this,
			FName(TEXT("/Game/Mapas/L_RaceMap")),
			true,
			TEXT("listen")
		);
	}
	else if (Action == 2)
	{
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->ClientTravel(PendingAddress, TRAVEL_Absolute);
		}
	}
	else if (Action == 3)
	{
		UKismetSystemLibrary::QuitGame(
			this,
			PlayerController,
			EQuitPreference::Quit,
			false
		);
	}
}

