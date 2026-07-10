// TrainingDummy.h
// 訓練假人：擁有 GAS 生命值，被火球命中扣血；血歸零後短暫失能再原位重置。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "TrainingDummy.generated.h"

class UAbilitySystemComponent;
class UParryLabAttributeSet;
class UCapsuleComponent;
class UStaticMeshComponent;
struct FOnAttributeChangeData;

UCLASS()
class PARRYLAB_API ATrainingDummy : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATrainingDummy();

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	/** 碰撞（根元件，火球命中它） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;

	/** 可見外觀 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	/** GAS 能力系統元件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	/** 生命屬性集 */
	UPROPERTY()
	UParryLabAttributeSet* AttributeSet;

	/** 死亡後多久重置（秒） */
	UPROPERTY(EditDefaultsOnly, Category = "Dummy")
	float RespawnDelay = 2.0f;

private:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void ResetDummy();

	FTimerHandle ResetTimer;
	bool bIsDead = false;
};
