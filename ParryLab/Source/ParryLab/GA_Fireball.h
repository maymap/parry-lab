// GA_Fireball.h
// 火球 GameplayAbility：施放時在角色前方生成一顆火球投射物。
// 魔力消耗與冷卻於後續階段透過 GameplayEffect 加入（CommitAbility 已預留）。

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fireball.generated.h"

class AFireballProjectile;

UCLASS()
class PARRYLAB_API UGA_Fireball : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Fireball();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	/** 回傳冷卻標籤，供 GAS 判斷是否冷卻中 */
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

protected:
	/** 要生成的火球投射物類別（預設 AFireballProjectile） */
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> ProjectileClass;

	/** 從角色前方多遠處生成火球（拉遠一點，避免被角色身體擋住） */
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float SpawnForwardOffset = 160.f;

	/** 生成高度偏移（約胸口/頭部高度，讓它在鏡頭中清楚可見） */
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float SpawnHeightOffset = 70.f;

	/** 冷卻標籤容器（由 GetCooldownTags 回傳） */
	FGameplayTagContainer CooldownTagsContainer;
};
