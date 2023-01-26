// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITurnBased.generated.h"
/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UTurnBased : public UInterface
{
	GENERATED_BODY()
};

/** Interface for objects that are to be updated at the beginning and end of turns in a turn-based game */
class INFECTIONSTRATEGY_API ITurnBased
{
	GENERATED_BODY()

public:
	ITurnBased() {};

	/** Method to be called at the start of a turn.
	 * @param playerId - Id of the player whose turn is beginning
	*/
	virtual void OnTurnBegin(const int32 playerId) = 0;

	/** Method to be called at the start of a turn.
	 * @param playerId - Id of the player whose turn is ending
	*/
	virtual void OnTurnEnd(const int32 playerId) = 0;
};
