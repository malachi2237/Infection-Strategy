// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITurnBased.h"
#include "UnitState.generated.h"

DECLARE_DELEGATE_OneParam(FOnUnitDiedDelegate, const int32);

class AVehicleUnit;

UCLASS(notplaceable)
class AUnitState : public AActor, public ITurnBased
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitState();

	void OnTurnBegin(const int32 playerId);

	void OnTurnEnd(const int32 playerId);

	void OnMatchEnd(const int32 winnerId);

	bool SetVehicleOwner(AVehicleUnit& unit);

	bool SetPlayerOwner(const int32 owner);
	
	void AddHealth(int32 health);

	void RemoveHealth(int32 health);

	FOnUnitDiedDelegate OnUnitDied;

	AVehicleUnit* OwningUnit = nullptr;

	int32 OwningPlayer = -1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;
private:

	int32 MaxHealth = -1;

	int32 CurrentHealth = -1;
};
