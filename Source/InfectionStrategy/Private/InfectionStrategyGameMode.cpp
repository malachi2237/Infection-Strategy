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

AInfectionStrategyGameMode::AInfectionStrategyGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AInfectionStrategyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AInfectionStrategyGameMode::BeginPlay()
{
	Super::BeginPlay();

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
}

int32 AInfectionStrategyGameMode::EndTurn()
{
	auto playerController = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (playerController)
		playerController->OnTurnEnd(ActivePlayerId);

	for (auto& unit : Vehicles)
	{
		if (unit.IsValid())
			unit->OnTurnEnd(ActivePlayerId);

	}

	TileSystem->OnTurnEnd(ActivePlayerId);

	// Next turn begins
	TurnNumber += ActivePlayerId * 1;
	ActivePlayerId = !ActivePlayerId;

	if (playerController)
		playerController->OnTurnBegin(ActivePlayerId);

	for (auto& unit : Vehicles)
	{
		if (unit.IsValid())
			unit->OnTurnBegin(ActivePlayerId);
	}

	TileSystem->OnTurnBegin(ActivePlayerId);

	return ActivePlayerId;
}