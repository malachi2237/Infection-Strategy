// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitState.generated.h"

class AVehicleUnit;

UCLASS(notplaceable)
class AUnitState : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitState();

	bool RegisterUnit(AVehicleUnit& unit, const int32 owner);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;
private:	

	int32 UnitId = -1;

	AVehicleUnit* OwningUnit = nullptr;

	int32 OwningPlayer = -1;

	int32 MaxHealth = -1;

	int32 CurrentHealth = -1;
};
