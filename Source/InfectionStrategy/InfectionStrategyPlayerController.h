// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Deque.h"
#include "ITurnBased.h"
#include "InfectionStrategyPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UUserWidget;
class UVehicleWidget;
class AVehicleUnit;
class ATileActor;
enum class Neighbor;

UCLASS()
class AInfectionStrategyPlayerController : public APlayerController, public ITurnBased
{
	GENERATED_BODY()

public:
	AInfectionStrategyPlayerController();

	virtual void BeginPlay() override;
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/* TurnBased Interface*/
	virtual void OnTurnBegin(int32 player) override;
	virtual void OnTurnEnd(int32 player) override;

	void DeselectUnit();

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

	void OnSelectUnitReleased();

	void OnConfirmMoveReleased();

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> hudTemplate;

	UPROPERTY()
	UUserWidget* hudInstance;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UVehicleWidget> vehicleHudTemplate;

	UPROPERTY()
	UVehicleWidget* vehicleHudInstance;

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


