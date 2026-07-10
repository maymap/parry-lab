// GE_FireballDamage.h
// 火球命中傷害 GameplayEffect：瞬間對目標 Health 扣固定值。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_FireballDamage.generated.h"

UCLASS()
class PARRYLAB_API UGE_FireballDamage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_FireballDamage();
};
