// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InfectionStrategyPlayerState.generated.h"

class AUnitState;

/**
 * 
 */
UCLASS()
class AInfectionStrategyPlayerState : public APlayerState
{
	GENERATED_BODY()
	

private:
	TArray<AUnitState*> UnitArray;
};
