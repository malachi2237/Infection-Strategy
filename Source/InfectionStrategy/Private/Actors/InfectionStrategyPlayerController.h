// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Deque.h"
#include "ITurnBased.h"
#include "Neighbor.h"
#include "InfectionStrategyPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class AVehicleUnit;
class ATileActor;

/** PlayerController for InfectionStrategy. */
UCLASS()
class AInfectionStrategyPlayerController : public APlayerController, public ITurnBased
{
	GENERATED_BODY()

public:
	AInfectionStrategyPlayerController();

	/* TEMPORARY */
	void AssignID(const int32 id) { playerId = id; }

	virtual void BeginPlay() override;

	/** TurnBased Interface */
	virtual void OnTurnBegin(const int32 player) override;
	virtual void OnTurnEnd(const int32 player) override;
	/** End TurnBased Interface */

	/** Sets ownership of vehicle and performs some setup on it.
	 * @param unit - Unit to be claimed
	 * @param ownerId - Id of the player the vehicle is assigned to
	 */
	void ClaimUnit(AVehicleUnit* unit, const int32 ownerId);

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:



	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void OnPossess(APawn* InPawn) override;

	/** Checks if given movement is possible for the selected unit. If it is, adds it to MovevementQueue.
	 * @param direction - Direction movement should be attempted
	 */
	void TryTileMovement(const ENeighbor direction);

	/** Undo the last movement for the selected unit. */
	void UndoMovement();

	/** Clears all queued vehicle movement. */
	void UndoAllMovement();

	/** Selects a unit on which to perform actions. Passing nullptr will deselect the current unit.
	 * @param unit - unit to select
	 */
	void SelectUnit(AVehicleUnit* unit);

	/** A callback that selects the unit underneath the mouse. */
	void OnSelectUnitReleased();

	/** A callback to perform the movements collected on the currently selected unit. */
	void OnConfirmMoveReleased();

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Currently selected vehicle on which to perform actions */
	UPROPERTY()	
	AVehicleUnit* SelectedVehicle = nullptr;

private:
	/** Callback to move the camera vertically on the game world plane.
	 * @param direction - Indicates positive or negative movement on axis
	 */
	void OnMoveCameraVerticalPressed(const int32 direction);

	/** Callback to move the camera horizontally on the game world plane.
	 * @param direction - Indicates positive or negative movement on axis
	 */
	void OnMoveCameraHorizontalPressed(const int32 direction);

	/** Callback to stop vertical camera movement */
	void OnMoveCameraVerticalReleased();

	/** Callback to stop horizontal camera movement */
	void OnMoveCameraHorizontalReleased();

	/** Direction to move the camera vertically */
	int32 cameraMoveVert = 0;

	/** Direction to move the camera horizontally */
	int32 cameraMoveHoriz = 0;

	/** Are we in the state of moving a unit? */
	bool bIsMovingUnit = false;

	/** Are we in the state of targetting with a unit? */
	bool bIsTargeting = false;

	/** ID of the currently active player */
	int32 playerId = 0;

	/** Queue of movements to be performed by selected unit when moves confirmed */
	TDeque<ATileActor*> MovementQueue;
};


