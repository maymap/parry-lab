// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "ParryLabCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UAbilitySystemComponent;
class UParryLabAttributeSet;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AParryLabCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** GAS：能力系統元件（管理屬性、技能、效果） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	/** GAS：本角色的屬性集（Mana/Health） */
	UPROPERTY()
	UParryLabAttributeSet* AttributeSet;

	/** 每秒回復的魔力 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess = "true"))
	float ManaRegenRate = 30.f;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Fire (火球) Input Action — 綁左鍵 */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;

	/** 火球技能類別（預設 UGA_Fireball，可在 BP 覆寫） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> FireballAbility;

public:

	/** Constructor */
	AParryLabCharacter();

	//~ Begin IAbilitySystemInterface
	/** GAS 介面：回傳本角色的 AbilitySystemComponent */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

protected:

	/** 初始化 GAS actor info */
	virtual void BeginPlay() override;

	/** 每幀回魔並於螢幕顯示數值 */
	virtual void Tick(float DeltaSeconds) override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for fire (火球) input — 觸發火球技能 */
	void OnFire(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

