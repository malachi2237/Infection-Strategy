// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InfectionStrategyGameState.generated.h"

class AUnitState;

/**
 * 
 */
UCLASS()
class AInfectionStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameState();

	void AddUnit(AUnitState& unitState, const int32 playerId) {};

private:
	UPROPERTY()
	TArray<int32> TeamScores;
};
