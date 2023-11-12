// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnitState.h"
#include "VehicleUnit.h"

#include "InfectionStrategyGameState.h"

// Sets default values
AUnitState::AUnitState()
{

}

void AUnitState::OnTurnBegin(const int32 playerId)
{

}

void AUnitState::OnTurnEnd(const int32 playerId)
{

}

void AUnitState::OnMatchEnd(const int32 winnerId)
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

bool AUnitState::SetPlayerOwner(const int32 playerId)
{
	if (playerId >= 0)
	{
		OwningPlayer = playerId;
		return true;
	}
	
	return false;
}

bool AUnitState::SetVehicleOwner(AVehicleUnit& unit)
{
	if (!OwningUnit)
	{
		OwningUnit = &unit;

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

void AUnitState::AddHealth(int32 health)
{
	CurrentHealth += health;

	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}

void AUnitState::RemoveHealth(int32 health)
{
	CurrentHealth -= health;

	if (CurrentHealth <= 0)
	{
		OwningUnit->OnDeath();

		OnUnitDied.ExecuteIfBound(OwningPlayer);
	}
	else
		OwningUnit->OnDamageTaken();
}

