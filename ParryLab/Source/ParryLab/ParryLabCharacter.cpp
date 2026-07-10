// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParryLabCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ParryLabAttributeSet.h"
#include "GA_Fireball.h"
#include "Engine/Engine.h"
#include "ParryLab.h"

AParryLabCharacter::AParryLabCharacter()
{
	// 開啟每幀更新（用於回魔與螢幕顯示數值）
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 固定俯視（等角）攝影機：拉高拉遠、往下看、不隨滑鼠或角色旋轉，
	// 降低 3D 暈眩、看清火球飛行。
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;                     // 拉近一點，動作感更強
	CameraBoom->bUsePawnControlRotation = false;              // 不隨滑鼠旋轉
	CameraBoom->SetUsingAbsoluteRotation(true);               // 忽略角色轉向，維持固定角度
	CameraBoom->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f)); // 斜俯角 45 度（Hades/Diablo 那種 3/4 視角）
	CameraBoom->bDoCollisionTest = false;                     // 不因牆壁自動拉近

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// GAS：建立能力系統元件與屬性集
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 屬性集初始值在 UParryLabAttributeSet 建構子設定（Health/Mana 皆 100）
	AttributeSet = CreateDefaultSubobject<UParryLabAttributeSet>(TEXT("AttributeSet"));

	// 預設火球技能（可在角色 BP 覆寫）
	FireballAbility = UGA_Fireball::StaticClass();
}

void AParryLabCharacter::BeginPlay()
{
	Super::BeginPlay();

	// GAS：初始化 actor info（單機下 owner 與 avatar 都是自己）
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 授予火球技能（在具權限端）
		if (HasAuthority() && FireballAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(FireballAbility, 1, INDEX_NONE, this));
		}
	}
}

void AParryLabCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AbilitySystemComponent && AttributeSet)
	{
		// 回魔（不超過上限）
		const float NewMana = FMath::Min(
			AttributeSet->GetMana() + ManaRegenRate * DeltaSeconds,
			AttributeSet->GetMaxMana());
		AbilitySystemComponent->SetNumericAttributeBase(UParryLabAttributeSet::GetManaAttribute(), NewMana);

		// 螢幕常駐顯示數值（暫時除錯用；正式 HUD 為 task 5）
		if (GEngine)
		{
			const FString Msg = FString::Printf(
				TEXT("Mana %.0f/%.0f     Health %.0f/%.0f"),
				AttributeSet->GetMana(), AttributeSet->GetMaxMana(),
				AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
			GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Cyan, Msg);
		}
	}
}

UAbilitySystemComponent* AParryLabCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AParryLabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AParryLabCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AParryLabCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AParryLabCharacter::Look);

		// Fire（火球）
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AParryLabCharacter::OnFire);
		}
	}
	else
	{
		UE_LOG(LogParryLab, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AParryLabCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AParryLabCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AParryLabCharacter::OnFire(const FInputActionValue& Value)
{
	// 觸發火球技能（GAS 會處理魔力/冷卻檢查——於後續階段加入）
	if (AbilitySystemComponent && FireballAbility)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(FireballAbility);
	}
}

void AParryLabCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AParryLabCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AParryLabCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AParryLabCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
