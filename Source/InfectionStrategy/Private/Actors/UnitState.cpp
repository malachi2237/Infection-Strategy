// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnitState.h"
#include "VehicleUnit.h"

#include "InfectionStrategyGameState.h"

// Sets default values
AUnitState::AUnitState()
{

}

// Called when the game starts or when spawned
void AUnitState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnitState::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

bool AUnitState::RegisterUnit(AVehicleUnit& unit, const int32 ownerPlayer)
{
	if (!OwningUnit)
	{
		OwningUnit = &unit;
		OwningPlayer = ownerPlayer;

		MaxHealth = OwningUnit->GetMaxHealth();
		CurrentHealth = MaxHealth;


		if (AInfectionStrategyGameState* gs = Cast<AInfectionStrategyGameState>(GetWorld()->GetGameState()))
		{
			gs->AddUnit(*this, OwningPlayer);

			return true;
		}
	}

	return false;
}

