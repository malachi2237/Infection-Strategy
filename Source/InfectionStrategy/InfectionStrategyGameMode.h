// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TileActor.h"
#include "TileSystem.h"
#include "GameFramework/GameModeBase.h"
#include "InfectionStrategyGameMode.generated.h"

UCLASS(minimalapi)
class AInfectionStrategyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameMode();

	virtual void BeginPlay() override;

	UTileSystem* tileSystem = nullptr;

private:
	int activePlayerId = 0;
};



