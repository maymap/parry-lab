// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParryLabGameMode.h"
#include "TrainingDummy.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Engine/World.h"

AParryLabGameMode::AParryLabGameMode()
{
	// 預設自動生成 C++ 假人
	DummyClass = ATrainingDummy::StaticClass();
}

void AParryLabGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!DummyClass || NumDummies <= 0 || !World)
	{
		return;
	}

	// 以第一個 PlayerStart 當基準，把假人排在其前方
	TActorIterator<APlayerStart> StartIt(World);
	APlayerStart* Start = StartIt ? *StartIt : nullptr;

	const FVector BaseLoc = Start ? Start->GetActorLocation() : FVector::ZeroVector;
	const FVector Forward = Start ? Start->GetActorForwardVector() : FVector::ForwardVector;
	const FVector Right   = Start ? Start->GetActorRightVector()   : FVector::RightVector;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < NumDummies; ++i)
	{
		const float SideOffset = (i - (NumDummies - 1) * 0.5f) * SideSpacing;
		FVector Loc = BaseLoc + Forward * ForwardDistance + Right * SideOffset;
		Loc.Z = BaseLoc.Z + 100.f; // 抬高避免陷入地板（膠囊半高 100）

		World->SpawnActor<AActor>(DummyClass, Loc, FRotator::ZeroRotator, Params);
	}
}
