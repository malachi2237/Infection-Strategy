// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfectionStrategyGameMode.generated.h"

class UTileSystem;
class AVehicleUnit;

UCLASS(minimalapi)
class AInfectionStrategyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfectionStrategyGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int EndTurn();
	

	UTileSystem* tileSystem = nullptr;

private:
	int activePlayerId = 0;
	int turnNumber = 1;

	UPROPERTY()
	TArray<TWeakObjectPtr<AVehicleUnit>> vehicles;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AVehicleUnit> vehicleTemplate;
};



