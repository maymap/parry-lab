// GE_FireballCost.cpp

#include "GE_FireballCost.h"
#include "ParryLabAttributeSet.h"

UGE_FireballCost::UGE_FireballCost()
{
	// 瞬間效果：一次扣完，不持續
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 對 Mana 做 -10 的加法修改（降低消耗，讓火球更好連發）
	FGameplayModifierInfo Mod;
	Mod.Attribute = UParryLabAttributeSet::GetManaAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-10.f);
	Modifiers.Add(Mod);
}
