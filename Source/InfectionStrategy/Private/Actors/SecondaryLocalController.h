// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InfectionStrategyPlayerController.h"
#include "SecondaryLocalController.generated.h"

/**
 * Controller for local players that are not Player One.
 */
UCLASS()
class ASecondaryLocalController : public AInfectionStrategyPlayerController
{
	GENERATED_BODY()

protected:
	// Begin PlayerController interface
	void OnPossess(APawn* InPawn) override;
	// End PlayerController interface};
};