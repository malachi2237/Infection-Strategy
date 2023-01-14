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

class INFECTIONSTRATEGY_API ITurnBased
{
	GENERATED_BODY()

public:
	ITurnBased() {};
	virtual void OnTurnBegin() = 0;
	virtual void OnTurnEnd() = 0;
};
