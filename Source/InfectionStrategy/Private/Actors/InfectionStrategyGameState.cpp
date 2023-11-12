// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InfectionStrategyGameState.h"
#include <Kismet/GameplayStatics.h>
#include "InfectionStrategyPlayerState.h"
#include "Actors/SecondaryLocalController.h"
#include "TileSystem.h"
#include "VehicleUnit.h"
#include "UnitState.h"

AInfectionStrategyGameState::AInfectionStrategyGameState()
{

}

void AInfectionStrategyGameState::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsGameWorld() && VehicleTemplate)
		{
			TileSystem = GetWorld()->SpawnActor<ATileSystem>(TileSystemClass);

			for (int i = 0, j = TileSystem->GridWidth - 1; i < 5; i++, j--)
			{
				/* Place first player's units. */
				FVector location = TileSystem->GetLocationAtTile(i, 0) + FVector(0, 0, 25.0f);
				FRotator rotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f));

				AVehicleUnit* newUnit = (AVehicleUnit*)GetWorld()->SpawnActor<AVehicleUnit>(VehicleTemplate, location, rotator);

				if (newUnit != nullptr && newUnit->UnitState != nullptr)
				{
					// spawn unit through PlayerState, keep track of them on the player
					if (AInfectionStrategyPlayerState* ps = Cast<AInfectionStrategyPlayerState>(PlayerArray[0]))
						ps->AddUnit(newUnit->UnitState);
					TileSystem->OccupyTile(i, 0);
				}

				/* Place second player's units. */
				location = TileSystem->GetLocationAtTile(j, TileSystem->GridHeight - 1) + FVector(0, 0, 25.0f);
				rotator = FRotator::ZeroRotator;

				newUnit = (AVehicleUnit*)GetWorld()->SpawnActor<AVehicleUnit>(VehicleTemplate, location, rotator);

				if (newUnit != nullptr)
				{
					// spawn unit through PlayerState, keep track of them on the player
					if (AInfectionStrategyPlayerState* ps = Cast<AInfectionStrategyPlayerState>(PlayerArray[1]))
						ps->AddUnit(newUnit->UnitState);
					TileSystem->OccupyTile(j, TileSystem->GridHeight - 1);
				}
			}
		}
}

void AInfectionStrategyGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

int32 AInfectionStrategyGameState::EndTurn()
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

	TileSystem->OnTurnEnd(ActivePlayerId);

	// Next turn begins
	ActivePlayerId = (ActivePlayerId + 1) % 2;

	StartTurn();

	return ActivePlayerId;
}

void AInfectionStrategyGameState::StartTurn()
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

	TileSystem->OnTurnBegin(ActivePlayerId);
}


void AInfectionStrategyGameState::AddUnit(AUnitState& unitState, const int32 playerId)
{
	/*if (TeamScores.IsValidIndex(playerId))
	{
		TeamScores[playerId]++;
		unitState.OnUnitDied.BindUObject(this, &AInfectionStrategyGameState::DecrementScore);
	}*/
}

void AInfectionStrategyGameState::DecrementScore(const int32 playerId)
{
	//if (TeamScores.IsValidIndex(playerId))
	//{
	//	TeamScores[playerId]--;
	//	if (TeamScores[playerId] <= 0)
	//		EndMatchForPlayers(playerId == 0 ? 1 : 0);
	//}
}

void AInfectionStrategyGameState::EndMatchForPlayers(const int32 winner)
{
}

void AInfectionStrategyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	PlayerState->SetPlayerId(PlayerArray.Num() - 1);
}


void AInfectionStrategyGameState::EndMatch(int32 winnerId)
{

}