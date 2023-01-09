// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class INFECTIONSTRATEGY_API ITargetable
{
public:
	ITargetable();
	virtual ~ITargetable() {}
	virtual void Target() = 0;
	virtual void Untarget() = 0;
};
