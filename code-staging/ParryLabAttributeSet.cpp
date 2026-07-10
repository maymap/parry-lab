// ParryLabAttributeSet.cpp

#include "ParryLabAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UParryLabAttributeSet::UParryLabAttributeSet()
{
	// 初始值。實際遊玩時建議改由 GameplayEffect（在編輯器設定）初始化，
	// 這裡給預設值是為了單機測試也能直接有滿血滿魔。
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMana(100.0f);
	InitMaxMana(100.0f);
}

void UParryLabAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 即使本 demo 為單機，仍照 GAS 慣例設定 replication，方便日後擴充連線。
	DOREPLIFETIME_CONDITION_NOTIFY(UParryLabAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UParryLabAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UParryLabAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UParryLabAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UParryLabAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UParryLabAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 直接 SetXxx（例如 UI 或初始化）時的夾值。
	ClampAttribute(Attribute, NewValue);
}

void UParryLabAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// GameplayEffect 實際結算後（例如火球傷害、魔力消耗）再夾一次，確保不超界。
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}

void UParryLabAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParryLabAttributeSet, Health, OldHealth);
}

void UParryLabAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParryLabAttributeSet, MaxHealth, OldMaxHealth);
}

void UParryLabAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParryLabAttributeSet, Mana, OldMana);
}

void UParryLabAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParryLabAttributeSet, MaxMana, OldMaxMana);
}
