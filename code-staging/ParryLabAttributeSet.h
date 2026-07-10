// ParryLabAttributeSet.h
// GAS 屬性集：定義玩家/假人的 Health、Mana。
// 對應 tasks.md 2.1、design.md「AttributeSet 用最小 C++，其餘 Blueprint」。
//
// 放置位置：<專案>/Source/ParryLab/  （與專案同模組名 ParryLab）
// 若你的專案模組名不是 ParryLab，請把下方所有 PARRYLAB_API 與類別前綴一起改掉。

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ParryLabAttributeSet.generated.h"

// GAS 標準存取器巨集：一次產生 Get/Set/Init 與 Attribute getter。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 本專案唯一的 AttributeSet。
 * 玩家角色使用 Mana（火球消耗）；訓練假人使用 Health（受擊扣血）。
 * 為求簡單，兩者共用同一個 AttributeSet，各自只實際用到需要的屬性。
 */
UCLASS()
class PARRYLAB_API UParryLabAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UParryLabAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// --- Health（假人生命值）---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UParryLabAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UParryLabAttributeSet, MaxHealth)

	// --- Mana（玩家魔力）---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UParryLabAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UParryLabAttributeSet, MaxMana)

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	// 依當前 Max 值把 Health/Mana 夾在 [0, Max]，避免超出或負值。
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
