// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectionStrategyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "InfectionStrategyPlayerController.h"
#include "InfectionStrategyCharacter.h"
#include "UObject/ConstructorHelpers.h"

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

	for (int i = 0; i < 5; i++)
	{
		const FVector location = tileSystem->GetLocationAt(i, 0) + FVector(0,0,25.0f);
		const FRotator rotator = FRotator::ZeroRotator;

		AVehicleUnit *newUnit = (AVehicleUnit *)GetWorld()->SpawnActor<AVehicleUnit>(AVehicleUnit::StaticClass(), location, rotator);

		if (newUnit != nullptr)
		{
			newUnit->SetOwner(0);
			tileSystem->OccupyTile(i, 0);
		}
	}
}

int AInfectionStrategyGameMode::EndTurn()
{
	activePlayerId = !activePlayerId;

	return activePlayerId;
}