// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ITurnBased.h"
#include "InfectionStrategyPlayerState.generated.h"

class AUnitState;

/**
 *
 */

UCLASS()
class AInfectionStrategyPlayerState : public APlayerState, public ITurnBased
{
	GENERATED_BODY()

public:
	void AddUnit(AUnitState* unit);

	void OnTurnBegin(const int32 playerId);

	void OnTurnEnd(const int32 playerId);

	void OnMatchEnd(const int32 winnerId);

private:
	TArray<TWeakObjectPtr<AUnitState>>UnitArray;
};
