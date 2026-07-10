// TrainingDummy.cpp

#include "TrainingDummy.h"
#include "AbilitySystemComponent.h"
#include "ParryLabAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"

ATrainingDummy::ATrainingDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	// 膠囊碰撞當根（火球會撞到它）
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->InitCapsuleSize(50.f, 100.f);
	Capsule->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Capsule;

	// 可見外觀：引擎內建圓柱
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(Capsule);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(CylinderMesh.Object);
		// 圓柱預設約 1m 高；拉高並下移對齊膠囊
		MeshComp->SetRelativeScale3D(FVector(1.f, 1.f, 2.f));
		MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	}

	// GAS
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UParryLabAttributeSet>(TEXT("AttributeSet"));
}

void ATrainingDummy::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 監聽 Health 變化，歸零時觸發重置
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UParryLabAttributeSet::GetHealthAttribute())
			.AddUObject(this, &ATrainingDummy::OnHealthChanged);
	}
}

UAbilitySystemComponent* ATrainingDummy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATrainingDummy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (bIsDead)
	{
		return;
	}

	if (Data.NewValue <= 0.f)
	{
		bIsDead = true;

		// 短暫失能：隱藏 + 關碰撞
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ResetTimer, this, &ATrainingDummy::ResetDummy, RespawnDelay, false);
		}
	}
}

void ATrainingDummy::ResetDummy()
{
	// 原位重置滿血
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->SetNumericAttributeBase(
			UParryLabAttributeSet::GetHealthAttribute(),
			AttributeSet->GetMaxHealth());
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsDead = false;
}
