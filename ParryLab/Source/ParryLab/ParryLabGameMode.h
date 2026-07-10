// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ParryLabGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AParryLabGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Constructor */
	AParryLabGameMode();

protected:
	/** 開場自動生成訓練假人（免手動擺放） */
	virtual void BeginPlay() override;

	/** 要自動生成的假人類別 */
	UPROPERTY(EditDefaultsOnly, Category = "Dummies")
	TSubclassOf<AActor> DummyClass;

	/** 生成幾個假人 */
	UPROPERTY(EditDefaultsOnly, Category = "Dummies")
	int32 NumDummies = 3;

	/** 假人排在出生點前方多遠 */
	UPROPERTY(EditDefaultsOnly, Category = "Dummies")
	float ForwardDistance = 700.f;

	/** 假人之間的左右間距 */
	UPROPERTY(EditDefaultsOnly, Category = "Dummies")
	float SideSpacing = 300.f;
};



