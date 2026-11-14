// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeyondTheFallCharacter.h"

#include "BeyondTheFall.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"


ABeyondTheFallCharacter::ABeyondTheFallCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USoundWave> JumpAsset(
	TEXT("/Game/Audio_SFX/salto.salto")
);

	if (JumpAsset.Succeeded())
	{
		JumpSound = JumpAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> FootstepAsset(
	TEXT("/Game/Audio_SFX/Pasos.Pasos")
);

	if (FootstepAsset.Succeeded())
	{
		FootstepSound = FootstepAsset.Object;
	}
	
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ABeyondTheFallCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CurrentStamina = MaxStamina;
	}
}

void ABeyondTheFallCharacter::EndPlay(
	const EEndPlayReason::Type EndPlayReason
)
{
	if (SprintInputSubsystem && SprintMappingContext)
	{
		SprintInputSubsystem->RemoveMappingContext(SprintMappingContext);
	}

	Super::EndPlay(EndPlayReason);
}

void ABeyondTheFallCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
)
{
	if (UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APlayerController* PlayerController =
			Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				SprintInputSubsystem =
					ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
						LocalPlayer
					);

				if (SprintInputSubsystem)
				{
					SprintAction = NewObject<UInputAction>(this);
					SprintAction->ValueType = EInputActionValueType::Boolean;

					SprintMappingContext = NewObject<UInputMappingContext>(this);
					SprintMappingContext->MapKey(SprintAction, EKeys::LeftShift);

					SprintInputSubsystem->AddMappingContext(SprintMappingContext, 0);

					EnhancedInputComponent->BindAction(
						SprintAction,
						ETriggerEvent::Started,
						this,
						&ABeyondTheFallCharacter::StartSprint
					);

					EnhancedInputComponent->BindAction(
						SprintAction,
						ETriggerEvent::Completed,
						this,
						&ABeyondTheFallCharacter::StopSprint
					);

					EnhancedInputComponent->BindAction(
						SprintAction,
						ETriggerEvent::Canceled,
						this,
						&ABeyondTheFallCharacter::StopSprint
					);
				}
			}
		}

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&ACharacter::Jump
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&ACharacter::StopJumping
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&ABeyondTheFallCharacter::Move
		);

		EnhancedInputComponent->BindAction(
			MouseLookAction,
			ETriggerEvent::Triggered,
			this,
			&ABeyondTheFallCharacter::Look
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&ABeyondTheFallCharacter::Look
		);
	}
	else
	{
		UE_LOG(
			LogBeyondTheFall,
			Error,
			TEXT("'%s' Failed to find an Enhanced Input component!"),
			*GetNameSafe(this)
		);
	}
}

void ABeyondTheFallCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ABeyondTheFallCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ABeyondTheFallCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ABeyondTheFallCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ABeyondTheFallCharacter::DoJumpStart()
{
	Jump();
}

void ABeyondTheFallCharacter::DoJumpEnd()
{
	StopJumping();
}

void ABeyondTheFallCharacter::StartSprint()
{
	ServerSetSprintRequested(true);
}

void ABeyondTheFallCharacter::StopSprint()
{
	ServerSetSprintRequested(false);
}

void ABeyondTheFallCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABeyondTheFallCharacter, CurrentStamina);
	DOREPLIFETIME(ABeyondTheFallCharacter, bStaminaExhausted);
	DOREPLIFETIME(ABeyondTheFallCharacter, bIsSprinting);
}

void ABeyondTheFallCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsLocallyControlled() && FootstepSound)
	{
		const float Speed = GetVelocity().Size2D();
		const bool bWalking = GetCharacterMovement()->IsMovingOnGround() && Speed > 100.0f;

		if (bWalking)
		{
			FootstepElapsed += DeltaSeconds;

			const float StepInterval = Speed > 650.0f ? 0.35f : 0.5f;

			if (FootstepElapsed >= StepInterval)
			{
				UGameplayStatics::PlaySound2D(this, FootstepSound);
				FootstepElapsed = 0.0f;
			}
		}
		else
		{
			FootstepElapsed = 0.0f;
		}
	}
	
	if (!HasAuthority())
	{
		return;
	}

	const bool bCanSprint =
		bSprintRequested
		&& !bStaminaExhausted
		&& CurrentStamina > 0.0f
		&& GetCharacterMovement()->MovementMode != MOVE_None;

	if (bCanSprint)
	{
		CurrentStamina = FMath::Max(
			0.0f,
			CurrentStamina - SprintDrainPerSecond * DeltaSeconds
		);

		if (CurrentStamina <= 0.0f)
		{
			bStaminaExhausted = true;
		}
	}
	else
	{
		CurrentStamina = FMath::Min(
			MaxStamina,
			CurrentStamina + StaminaRegenPerSecond * DeltaSeconds
		);

		if (bStaminaExhausted && CurrentStamina >= MaxStamina)
		{
			bStaminaExhausted = false;
		}
	}

	const bool bShouldSprint = bCanSprint && !bStaminaExhausted;

	if (bIsSprinting != bShouldSprint)
	{
		bIsSprinting = bShouldSprint;
		OnRep_IsSprinting();
	}
}

void ABeyondTheFallCharacter::ServerSetSprintRequested_Implementation(
	bool bRequested
)
{
	bSprintRequested = bRequested;
}

void ABeyondTheFallCharacter::OnRep_IsSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting
		? SprintSpeed
		: WalkSpeed;
}
void ABeyondTheFallCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (IsLocallyControlled() && JumpSound)
	{
		UGameplayStatics::PlaySound2D(this, JumpSound);
	}
}


