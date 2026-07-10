// GE_FireballDamage.cpp

#include "GE_FireballDamage.h"
#include "ParryLabAttributeSet.h"

UGE_FireballDamage::UGE_FireballDamage()
{
	// 瞬間效果
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 對 Health 做 -40 的加法修改
	FGameplayModifierInfo Mod;
	Mod.Attribute = UParryLabAttributeSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-40.f);
	Modifiers.Add(Mod);
}
