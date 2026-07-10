// GE_FireballCooldown.h
// 火球冷卻 GameplayEffect：施放後授予「Cooldown.Fireball」標籤一段時間，
// 期間 GAS 會擋下再次施放。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "GE_FireballCooldown.generated.h"

// 冷卻期間掛在施法者身上的標籤（GA_Fireball 會用它判斷冷卻中）
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Fireball);

UCLASS()
class PARRYLAB_API UGE_FireballCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_FireballCooldown();
};
