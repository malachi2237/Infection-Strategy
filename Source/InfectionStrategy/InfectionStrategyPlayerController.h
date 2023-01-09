// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Deque.h"
#include "TileSystem.h"
#include "VehicleUnit.h"
#include "InfectionStrategyPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AInfectionStrategyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInfectionStrategyPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	void StartTurn(int32 player);
	void EndTurn();
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	AVehicleUnit* selectedVehicle = nullptr;
	void TryTileMovement(Neighbor direction);
	void UndoMovement();
	void UndoMovement(int32 moveCount);
	void SelectUnit(AVehicleUnit *unit);


	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	//void OnSetDestinationPressed();
	//void OnSetDestinationReleased();
	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

	//void OnSelectUnitPressed();
	void OnSelectUnitReleased();

	void OnConfirmMoveReleased();

private:
	int32 cameraMoveVert = 0;
	int32 cameraMoveHoriz = 0; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	void OnMoveCameraVerticalPressed(int32 direction);
	void OnMoveCameraHorizontalPressed(int32 direction);

	void OnMoveCameraVerticalReleased();
	void OnMoveCameraHorizontalReleased();

	int32 playerId = 0;
	TDeque<ATileActor*> movementQueue;
};


