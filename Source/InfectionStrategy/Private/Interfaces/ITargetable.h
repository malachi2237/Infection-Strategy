// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITargetable.generated.h"


UINTERFACE(MinimalAPI, Blueprintable) 
class UTargetable : public UInterface
{
	GENERATED_BODY()
};

/** The interface for targetable objects. */
class INFECTIONSTRATEGY_API ITargetable
{
	GENERATED_BODY()
public:
	ITargetable() {};

	/** Call when object is being targeted. */
	virtual void Target() = 0;

	/** Call when the object was targeted but is no longer. */
	virtual void Untarget() = 0;
};
