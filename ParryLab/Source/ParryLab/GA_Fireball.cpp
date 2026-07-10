// GA_Fireball.cpp

#include "GA_Fireball.h"
#include "FireballProjectile.h"
#include "GE_FireballCost.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "TimerManager.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Fireball, "Cooldown.Fireball");

UGA_Fireball::UGA_Fireball()
{
	// 每個角色一份技能實例
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// 預設用 C++ 的火球類別，之後可在編輯器換成帶特效的 BP 子類
	ProjectileClass = AFireballProjectile::StaticClass();

	// 魔力消耗：GAS 用它自動判斷「魔力足夠才可施放」
	CostGameplayEffectClass = UGE_FireballCost::StaticClass();

	// 冷卻：以 loose gameplay tag 實作（見 ActivateAbility）；GetCooldownTags 回傳此標籤供 CheckCooldown 判斷
	CooldownTagsContainer.AddTag(TAG_Cooldown_Fireball);
}

const FGameplayTagContainer* UGA_Fireball::GetCooldownTags() const
{
	return &CooldownTagsContainer;
}

void UGA_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	// CommitAbility 會套用 Cost/Cooldown（此階段尚未設定，故必定成功；3-2/3-3 加入後即生效）
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;

	// 只在具權限端生成（單機 PIE 即符合）
	if (Avatar && ProjectileClass && ActorInfo->IsNetAuthority())
	{
		// 瞄準方向：pawn 用控制器視角，否則用自身朝向
		FRotator SpawnRot = Avatar->GetActorRotation();
		if (const APawn* Pawn = Cast<APawn>(Avatar))
		{
			SpawnRot = Pawn->GetControlRotation();
		}

		const FVector SpawnLoc = Avatar->GetActorLocation()
			+ SpawnRot.Vector() * SpawnForwardOffset
			+ FVector(0.f, 0.f, SpawnHeightOffset);

		FActorSpawnParameters Params;
		Params.Owner = Avatar;
		Params.Instigator = Cast<APawn>(Avatar);
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (UWorld* World = GetWorld())
		{
			World->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLoc, SpawnRot, Params);
		}
	}

	// 手動冷卻：給施法者掛冷卻標籤，CooldownDuration 秒後自動移除。
	// （UE5.8 在 GameplayEffect 建構子加 TargetTags 元件會 crash，故改用 loose tag）
	if (UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr)
	{
		ASC->AddLooseGameplayTag(TAG_Cooldown_Fireball);
		if (UWorld* World = GetWorld())
		{
			TWeakObjectPtr<UAbilitySystemComponent> WeakASC(ASC);
			World->GetTimerManager().SetTimer(CooldownTimerHandle,
				[WeakASC]()
				{
					if (WeakASC.IsValid())
					{
						WeakASC->RemoveLooseGameplayTag(TAG_Cooldown_Fireball);
					}
				},
				CooldownDuration, false);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
