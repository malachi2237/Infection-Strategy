// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InfectionStrategyPlayerState.h"
#include "InfectionStrategyPlayerController.h"
#include "UnitState.h"

void AInfectionStrategyPlayerState::OnTurnBegin(const int32 playerId)
{
	auto controller = (AInfectionStrategyPlayerController*)GetOwningController();

	if (controller)
		controller->OnTurnBegin(playerId);

	for (int i = 0; i < UnitArray.Num(); i++)
	{
		if (UnitArray[i].IsValid())
			UnitArray[i]->OnTurnBegin(playerId);
	}
}

void AInfectionStrategyPlayerState::OnTurnEnd(const int32 playerId)
{
	auto controller = (AInfectionStrategyPlayerController*)GetOwningController();

	if (controller)
		controller->OnTurnEnd(playerId);

	for (int i = 0; i < UnitArray.Num(); i++)
	{
		if (UnitArray[i].IsValid())
			UnitArray[i]->OnTurnEnd(playerId);
	}
}

void AInfectionStrategyPlayerState::OnMatchEnd(const int32 winnerId)
{
	auto controller = (AInfectionStrategyPlayerController*)GetOwningController();

	if (controller)
		controller->OnMatchEnd(winnerId);

	for (int i = 0; i < UnitArray.Num(); i++)
	{
		if (UnitArray[i].IsValid())
			UnitArray[i]->OnMatchEnd(winnerId);
	}
}


void AInfectionStrategyPlayerState::AddUnit(AUnitState *unit)
{
	unit->SetPlayerOwner(GetPlayerId());

	UnitArray.Add(unit);
}