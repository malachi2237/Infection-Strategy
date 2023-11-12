// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InfectionStrategyGameState.generated.h"

DECLARE_DELEGATE_TwoParams(FOnScoreChangedDelegate, const int32, const int32);

class AUnitState;
class ATileSystem;
class AVehicleUnit;

/**
 * 
 */
UCLASS()
class AInfectionStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameState();


	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	void AddPlayerState(APlayerState* PlayerState) override;

	/**
	 * Ends the current player's turn for all Actors and set the next player as current.
	 * @return The Id of the player whose turn is next.
	 */
	UFUNCTION(BlueprintCallable)
	int32 EndTurn();

	/** Begins the current player's turn for all Actors. */
	UFUNCTION()
	void StartTurn();

	/** Returns the Id of the player whose turn it currently is. */
	UFUNCTION()
	int32 GetActivePlayerId() { return ActivePlayerId; }

	void AddUnit(AUnitState& unitState, const int32 playerId);

	void EndMatchForPlayers(int32 winner);

	FOnScoreChangedDelegate OnScoreChanged;

	/** The current ATileSystem in use. */
	UPROPERTY()
	ATileSystem* TileSystem = nullptr;

	UPROPERTY()
	TSubclassOf<ATileSystem> TileSystemClass;

	/** Template used to spawn units.
	 * Will be removed when players can select their own unit types
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVehicleUnit> VehicleTemplate;
protected:

	UFUNCTION()
	void DecrementScore(int32 playerId);

private:
	/** Player whose turn it currently is */
	int32 ActivePlayerId = 0;

	/** Count of the current turn. Increments after both players play their turns. */
	uint32 TurnNumber = 1;




	virtual void EndMatch(int32 winnerId);
};
