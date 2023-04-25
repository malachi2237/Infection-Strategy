// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfectionStrategyGameMode.generated.h"

class UTileSystem;
class AVehicleUnit;

/* The default game mode for the project. Establishes rules and Pawns. */
UCLASS(minimalapi)
class AInfectionStrategyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameMode();

	virtual void BeginPlay() override;

	/**
	 * Ends the current player's turn for all Actors.
	 * @return The Id of the player whose turn is next.
	 */
	UFUNCTION(BlueprintCallable)
	int32 EndTurn();

	UFUNCTION()
	void StartTurn();
	
	/** The current UTileSystem in use. */
	UPROPERTY()
	UTileSystem* TileSystem = nullptr;

private:
	/** Player whose turn it currently is */
	int32 ActivePlayerId = 0;

	/** Count of the current turn. Increments after both players play their turns. */
	uint32 TurnNumber = 1;

	/** Collection of units being used*/
	UPROPERTY()
	TArray<TWeakObjectPtr<AVehicleUnit>> Vehicles;

	/** Template used to spawn units.
	 * Will be removed when players can select their own unit types
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVehicleUnit> VehicleTemplate;
};



