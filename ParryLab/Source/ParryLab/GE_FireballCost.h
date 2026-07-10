// GE_FireballCost.h
// 火球的魔力消耗 GameplayEffect：施放時瞬間扣除固定 Mana。
// GAS 會用它自動判斷「魔力足夠才可施放」。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_FireballCost.generated.h"

UCLASS()
class PARRYLAB_API UGE_FireballCost : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_FireballCost();
};
