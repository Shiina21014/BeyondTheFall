#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BeyondTFRaceHUD.generated.h"

class SWidget;
class UTextBlock;
class UButton;
class UProgressBar;
class USoundWave;

UCLASS()
class BEYONDTHEFALL_API UBeyondTFRaceHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UBeyondTFRaceHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime
	) override;


private:
	
	UPROPERTY()
	TObjectPtr<USoundWave> CountdownSound;
	
	UPROPERTY()
	TObjectPtr<USoundWave> VictorySound;
	
	UPROPERTY()
	TObjectPtr<USoundWave> DefeatSound;

	bool bDefeatSoundPlayed = false;

	bool bVictorySoundPlayed = false;
	
	bool bCountdownSoundPlayed = false;
	
	UPROPERTY()
	TObjectPtr<UButton> StartButton;

	UFUNCTION()
	void OnStartClicked();
	
	UPROPERTY()
	TObjectPtr<UTextBlock> AttemptsText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> PositionText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> CheckpointNoticeText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> PitNoticeText;
	
	UPROPERTY()
	TObjectPtr<UProgressBar> StaminaBar;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> CountdownText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> WaitingText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> ResultsText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> DisqualifiedText;
	
	UPROPERTY()
	TObjectPtr<UButton> RestartButton;

	UPROPERTY()
	TObjectPtr<UButton> ExitButton;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnExitClicked();

	bool bResultsInputConfigured = false;


};