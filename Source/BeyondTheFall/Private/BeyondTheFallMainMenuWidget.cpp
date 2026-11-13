#include "BeyondTheFallMainMenuWidget.h"
#include "Public/BeyondTheFallMainMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UBeyondTheFallMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree || WidgetTree->RootWidget)
    {
        return;
    }

    UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
    WidgetTree->RootWidget = RootPanel;

    UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    TitleText->SetText(FText::FromString(TEXT("BEYOND THE FALL")));

    FSlateFontInfo TitleFont = TitleText->GetFont();
    TitleFont.Size = 48;
    TitleText->SetFont(TitleFont);

    UCanvasPanelSlot* TitleSlot = RootPanel->AddChildToCanvas(TitleText);
    TitleSlot->SetAnchors(FAnchors(0.5f, 0.30f));
    TitleSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    TitleSlot->SetAutoSize(true);

    StartButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

    UTextBlock* StartText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    StartText->SetText(FText::FromString(TEXT("JUGAR")));

    FSlateFontInfo StartFont = StartText->GetFont();
    StartFont.Size = 24;
    StartText->SetFont(StartFont);

    StartButton->SetContent(StartText);
    StartButton->OnClicked.AddDynamic(this, &UBeyondTheFallMainMenuWidget::HandleStartClicked);

    UCanvasPanelSlot* StartSlot = RootPanel->AddChildToCanvas(StartButton);
    StartSlot->SetAnchors(FAnchors(0.5f, 0.50f));
    StartSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    StartSlot->SetOffsets(FMargin(-160.0f, -30.0f, 320.0f, 60.0f));

    ExitButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

    UTextBlock* ExitText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    ExitText->SetText(FText::FromString(TEXT("SALIR")));

    FSlateFontInfo ExitFont = ExitText->GetFont();
    ExitFont.Size = 24;
    ExitText->SetFont(ExitFont);

    ExitButton->SetContent(ExitText);
    ExitButton->OnClicked.AddDynamic(this, &UBeyondTheFallMainMenuWidget::HandleExitClicked);

    UCanvasPanelSlot* ExitSlot = RootPanel->AddChildToCanvas(ExitButton);
    ExitSlot->SetAnchors(FAnchors(0.5f, 0.62f));
    ExitSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    ExitSlot->SetOffsets(FMargin(-160.0f, -30.0f, 320.0f, 60.0f));
}

void UBeyondTheFallMainMenuWidget::HandleStartClicked()
{
    RemoveFromParent();
}

void UBeyondTheFallMainMenuWidget::HandleExitClicked()
{
    UKismetSystemLibrary::QuitGame(
        this,
        GetOwningPlayer(),
        EQuitPreference::Quit,
        false
    );
}