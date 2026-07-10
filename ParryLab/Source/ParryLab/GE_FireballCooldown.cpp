// GE_FireballCooldown.cpp

#include "GE_FireballCooldown.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Fireball, "Cooldown.Fireball");

UGE_FireballCooldown::UGE_FireballCooldown()
{
	// 持續 3 秒的冷卻
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(3.0f);

	// 冷卻期間授予 Cooldown.Fireball 標籤給施法者（UE5.3+ 以元件方式設定）
	UTargetTagsGameplayEffectComponent& TagsComp = FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagChanges;
	TagChanges.Added.AddTag(TAG_Cooldown_Fireball);
	TagsComp.SetAndApplyTargetTagChanges(TagChanges);
}
