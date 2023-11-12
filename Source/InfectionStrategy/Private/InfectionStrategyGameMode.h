// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SecondaryLocalController.h"
#include "InfectionStrategyGameMode.generated.h"

class ATileSystem;
class AVehicleUnit;

/* The default game mode for the project. Establishes rules and Pawns. */
UCLASS(minimalapi)
class AInfectionStrategyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameMode();

	virtual void BeginPlay() override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	/** The player controller class that should be used for any additional local players */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = InfectionStrategyClasses)
		TSubclassOf<ASecondaryLocalController> SecondPlayerControllerClass;

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = InfectionStrategyClasses)
		TSubclassOf<ATileSystem> TileSystemClass;

	/** Template used to spawn units.
	 * Will be removed when players can select their own unit types
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVehicleUnit> VehicleTemplate;

private:
	UFUNCTION()
		virtual void CheckScoreForWinner(const int32 playerOneScore, const int32 playerTwoScore);
};



