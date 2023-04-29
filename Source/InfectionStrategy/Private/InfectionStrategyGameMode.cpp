// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectionStrategyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "InfectionStrategyPlayerController.h"
#include "InfectionStrategyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TileActor.h"
#include "TileSystem.h"
#include "VehicleUnit.h"
#include <Kismet/GameplayStatics.h>
#include "Actors/SecondaryLocalController.h"

AInfectionStrategyGameMode::AInfectionStrategyGameMode()
{

}

void AInfectionStrategyGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerClass = SecondPlayerControllerClass;

	if (const UWorld* world = GetWorld())
		UGameplayStatics::CreatePlayer(world, 1);

	if (VehicleTemplate)
	{
		for (int i = 0, j = TileSystem->GridWidth - 1; i < 5; i++, j--)
		{
			/* Place first players units. */
			FVector location = TileSystem->GetLocationAtTile(i, 0) + FVector(0, 0, 25.0f);
			FRotator rotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f));

			AVehicleUnit* newUnit = (AVehicleUnit*)GetWorld()->SpawnActor<AVehicleUnit>(VehicleTemplate, location, rotator);

			if (newUnit != nullptr)
			{
				newUnit->SetPlayerOwner(0);
				TileSystem->OccupyTile(i, 0);

				Vehicles.Add(TWeakObjectPtr<AVehicleUnit>(newUnit));
			}

			/* Place second player's units. */
			location = TileSystem->GetLocationAtTile(j, TileSystem->GridHeight - 1) + FVector(0, 0, 25.0f);
			rotator = FRotator::ZeroRotator;

			newUnit = (AVehicleUnit*)GetWorld()->SpawnActor<AVehicleUnit>(VehicleTemplate, location, rotator);

			if (newUnit != nullptr)
			{
				newUnit->SetPlayerOwner(1);
				TileSystem->OccupyTile(j, TileSystem->GridHeight - 1);

				Vehicles.Add(TWeakObjectPtr<AVehicleUnit>(newUnit));
			}
		}
	}

	if (UWorld* world = GetWorld())
	{
		for (int i = 0; i < UGameplayStatics::GetNumLocalPlayerControllers(this); i++)
		{
			auto playerController = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, i));

			if (playerController)
				playerController->AssignID(i);
		}
	}
}

int32 AInfectionStrategyGameMode::EndTurn()
{
	if (UWorld* world = GetWorld())
	{
		for (int i = 0; i < UGameplayStatics::GetNumLocalPlayerControllers(this); i++)
		{
			auto playerController = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, i));

			if (playerController)
				playerController->OnTurnEnd(ActivePlayerId);
		}
	}

	for (auto& unit : Vehicles)
	{
		if (unit.IsValid())
			unit->OnTurnEnd(ActivePlayerId);
	}

	TileSystem->OnTurnEnd(ActivePlayerId);

	// Next turn begins
	ActivePlayerId = (ActivePlayerId + 1) % 2;

	StartTurn();

	return ActivePlayerId;
}

void AInfectionStrategyGameMode::StartTurn()
{
	TurnNumber += ActivePlayerId * 1;

	if (UWorld* world = GetWorld())
	{
		for (int i = 0; i < UGameplayStatics::GetNumLocalPlayerControllers(this); i++)
		{
			auto playerController = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, i));

			if (playerController)
				playerController->OnTurnBegin(ActivePlayerId);
		}
	}

	for (auto& unit : Vehicles)
	{
		if (unit.IsValid())
			unit->OnTurnBegin(ActivePlayerId);
	}

	TileSystem->OnTurnBegin(ActivePlayerId);	
}