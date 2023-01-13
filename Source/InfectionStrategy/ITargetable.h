// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITargetable.generated.h"
/**
 * 
 */

UINTERFACE(MinimalAPI, Blueprintable) 
class UTargetable : public UInterface
{
	GENERATED_BODY()
};

class INFECTIONSTRATEGY_API ITargetable
{
	GENERATED_BODY()
public:
	ITargetable() {};
	virtual void Target() = 0;
	virtual void Untarget() = 0;
};
