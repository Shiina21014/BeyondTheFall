#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BeyondTFMenuWidget.generated.h"

class UButton;
class SWidget;
class UEditableTextBox;
class UTextBlock;
class UAudioComponent;
class USoundWave;
class UTexture2D;

UCLASS()
class BEYONDTHEFALL_API UBeyondTFMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBeyondTFMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BuildMenu();

	UPROPERTY()
	TObjectPtr<UButton> PlayButton;

	UPROPERTY()
	TObjectPtr<UButton> ExitButton;
	
	UPROPERTY()
	TObjectPtr<UButton> JoinButton;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> IpAddressInput;

	UPROPERTY()
	TObjectPtr<UTextBlock> JoinErrorText;
	
	UPROPERTY()
	TObjectPtr<USoundWave> MenuMusic;

	UPROPERTY()
	TObjectPtr<UAudioComponent> MenuMusicComponent;
	
	UPROPERTY()
	TObjectPtr<USoundWave> ButtonSound;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> MenuBackground;

	FTimerHandle MenuActionTimer;

	bool bMenuActionPending = false;
	FString PendingAddress;

	void QueueMenuAction(int32 Action);
	void ExecuteMenuAction(int32 Action);

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnExitClicked();
};
