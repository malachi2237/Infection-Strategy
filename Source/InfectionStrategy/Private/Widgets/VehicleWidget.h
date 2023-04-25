// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleWidget.generated.h"


class AVehicleUnit;

DECLARE_DELEGATE(FUiVehicleActionDelegate);

/** Base class for HUD for vehicle unit actions */
UCLASS()
class INFECTIONSTRATEGY_API UVehicleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Sets the vehicle the widget will interact with. */
	void SetSelectedVehicle(const AVehicleUnit* const unit) { SelectedVehicle = unit; }

	/** Returns the amount of tile movement remaining for the selected vehicle. */
	UFUNCTION(BlueprintCallable)
	int32 GetRemainingMovement() const;

	/** Deselect the vehicle and restore the widget to a deselected state */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Deselect();

	/** Callback for when the selected vehicle completes movement. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMovementCompleted();

	/** Delegate for when movement action is selected */
	FUiVehicleActionDelegate OnMovementSelected;

	/** Delegate for when attack action is selected */
	FUiVehicleActionDelegate OnAttackSelected;

	/** Delegate for when vehicle is deselected via the widget */
	FUiVehicleActionDelegate OnDeselect;

	/** Delegate for when an attack is confirmed via the widget */
	FUiVehicleActionDelegate OnAttack;

	/** Delegate for when movement is confirmed via the widget */
	FUiVehicleActionDelegate OnMove;

protected:
	/** Currently selected vehicle */
	UPROPERTY()
	const AVehicleUnit* SelectedVehicle;

	/** Executes OnMovementSelected delegate */
	UFUNCTION(BlueprintCallable)
	void SendOnMovementSelected();

	/** Executes OnAttackSelected delegate */
	UFUNCTION(BlueprintCallable)
	void SendOnAttackSelected();

	/** Executes OnDeselect delegate */
	UFUNCTION(BlueprintCallable)
	void SendOnDeselect();

	/** Executes OnAttack delegate */
	UFUNCTION(BlueprintCallable)
	void SendOnAttack();

	/** Executes OnMove delegate */
	UFUNCTION(BlueprintCallable)
	void SendOnMove();
};
