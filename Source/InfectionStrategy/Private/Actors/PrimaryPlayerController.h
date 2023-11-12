// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InfectionStrategyPlayerController.h"
#include "PrimaryPlayerController.generated.h"

/**
 *  Controller for the Player One. Distributes input to other local controllers.
 */
UCLASS()
class APrimaryPlayerController : public AInfectionStrategyPlayerController
{
	GENERATED_BODY()
	
protected:

	// Begin PlayerController interface
	virtual void SetupInputComponent() override;
	void OnPossess(APawn* InPawn) override;
	// End PlayerController interface

private:

	virtual void OnMatchEnd(const int32 winnerId) override;

	/** Retrieves a pointer to the player whose turn it is. */
	AInfectionStrategyPlayerController* GetCurrentPlayerController() const;

	/** Distributes tile movement input to the current player.
	 * @param direction - Direction movement should be attempted
	 */
	void HandleTileMovementInput(const ENeighbor direction);

	/** Distributes vehicle selection input to the current player. */
	void HandleSelectionInput();

	/** Distributes confirm input to the current player. */
	void HandleConfirmInput();

	/** Distributes vertical camera movement input to the current player.
	 * @param direction - Indicates positive or negative movement on axis
	 */
	void HandleCameraVerticalInput(const int32 direction);

	/** Distributes horizontal camera movement input to the current player.
	 * @param direction - Indicates positive or negative movement on axis
	 */
	void HandleCameraHorizontalInput(const int32 direction);

	/** Distributes cancel vertical camera movement input to the current player. */
	void HandleCameraVerticalCancelled();

	/** Distributes cancel horizontal camera movement input to the current player.  */
	void HandleCameraHorizontalCancelled();

	/** Distributes undo movement input input to the current player. */
	void HandleUndoMovementInput();
};
