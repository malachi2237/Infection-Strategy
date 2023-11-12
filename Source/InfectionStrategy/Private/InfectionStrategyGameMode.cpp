// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectionStrategyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "InfectionStrategyPlayerController.h"
#include "InfectionStrategyCharacter.h"
#include "TileSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "VehicleUnit.h"
#include "TileActor.h"


#include <Kismet/GameplayStatics.h>

#include "InfectionStrategyGameState.h"

AInfectionStrategyGameMode::AInfectionStrategyGameMode()
{

}


void AInfectionStrategyGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();


}

void AInfectionStrategyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetWorld()->IsGameWorld())
	{
		if (auto gs = Cast<AInfectionStrategyGameState>(GameState))
		{
			gs->OnScoreChanged.BindUObject(this, &AInfectionStrategyGameMode::CheckScoreForWinner);
			if (TileSystemClass)
				gs->TileSystemClass = TileSystemClass;
			if (VehicleTemplate)
				gs->VehicleTemplate = VehicleTemplate;
		}
	}

}

void AInfectionStrategyGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerClass = SecondPlayerControllerClass;

	if (const UWorld* world = GetWorld())
		UGameplayStatics::CreatePlayer(world, 1);
}



void AInfectionStrategyGameMode::CheckScoreForWinner(const int32 playerOneScore, const int32 playerTwoScore)
{
	//if (playerOneScore <= 0)
	//	EndMatch(1);
	//else if (playerTwoScore <= 0)
	//	EndMatch(0);
}

