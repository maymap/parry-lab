// FireballProjectile.cpp

#include "FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 球形碰撞當根元件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(35.f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnHit);
	RootComponent = CollisionComp;

	// 可見球體（用引擎內建 Sphere，免自備素材）
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(SphereMesh.Object);
		// 引擎 Sphere 直徑 100cm；縮到 ~70cm 對上碰撞半徑（比先前大、更明顯）
		MeshComp->SetRelativeScale3D(FVector(0.7f));
	}

	// 飛行：等速直線、不受重力
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// 3 秒後自動消失，避免飛太遠殘留
	InitialLifeSpan = 3.f;
}

void AFireballProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	// 階段 3-1：命中任何東西就銷毀（傷害於 3-4 加入）
	Destroy();
}
