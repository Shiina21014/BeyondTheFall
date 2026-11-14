// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BeyondTheFallCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class USoundWave;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class ABeyondTheFallCharacter : public ACharacter
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

public:
	ABeyondTheFallCharacter();

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Stamina")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Stamina")
	bool bStaminaExhausted = false;

	
	UFUNCTION(Server, Reliable)
	void ServerSetSprintRequested(bool bRequested);

protected:
	virtual void BeginPlay() override;
	virtual void OnJumped_Implementation() override;

	UPROPERTY()
	TObjectPtr<USoundWave> JumpSound;
	
	UPROPERTY()
	TObjectPtr<USoundWave> FootstepSound;

	float FootstepElapsed = 0.0f;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent
	) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void StartSprint();
	void StopSprint();


private:
	UFUNCTION()
	void OnRep_IsSprinting();

	UPROPERTY(ReplicatedUsing = OnRep_IsSprinting)
	bool bIsSprinting = false;

	bool bSprintRequested = false;
	
	UPROPERTY()
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> SprintMappingContext;

	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> SprintInputSubsystem;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float SprintDrainPerSecond = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenPerSecond = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float WalkSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float SprintSpeed = 850.0f;

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}
};


