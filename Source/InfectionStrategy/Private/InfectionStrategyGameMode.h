// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfectionStrategyGameMode.generated.h"

class UTileSystem;
class AVehicleUnit;
class ASecondaryLocalController;

/* The default game mode for the project. Establishes rules and Pawns. */
UCLASS(minimalapi)
class AInfectionStrategyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameMode();

	virtual void BeginPlay() override;

	/** Returns the Id of the player whose turn it currently is. */
	UFUNCTION()
	int32 GetActivePlayerId() { return ActivePlayerId; }

	/**
	 * Ends the current player's turn for all Actors and set the next player as current.
	 * @return The Id of the player whose turn is next.
	 */
	UFUNCTION(BlueprintCallable)
	int32 EndTurn();

	/** Begins the current player's turn for all Actors. */
	UFUNCTION()
	void StartTurn();

	/** The player controller class that should be used for any additional local players */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<ASecondaryLocalController> SecondPlayerControllerClass;

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



