// FireballProjectile.h
// 火球投射物：一顆朝前飛行、命中即銷毀的球體。傷害於後續階段加入。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PARRYLAB_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

protected:
	/** 球形碰撞（根元件，負責命中判定） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	/** 可見的球體外觀 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	/** 讓火球朝前飛行 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** 命中任何東西時呼叫 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);
};
