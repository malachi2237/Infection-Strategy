// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleWidget.generated.h"


class AVehicleUnit;


DECLARE_DELEGATE(FUiVehicleActionDelegate);
/**
 * 
 */
UCLASS()
class INFECTIONSTRATEGY_API UVehicleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSelectedVehicle(AVehicleUnit* unit) { selectedVehicle = unit; }

	UFUNCTION(BlueprintCallable)

	int32 GetRemainingMovement();

	FUiVehicleActionDelegate OnDeselect;
	FUiVehicleActionDelegate OnAttack;
	FUiVehicleActionDelegate OnMove;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Deselect();

protected:
	AVehicleUnit* selectedVehicle;

	UFUNCTION(BlueprintCallable)
	void SendOnDeselect();

	UFUNCTION(BlueprintCallable)
	void SendOnAttack();

	UFUNCTION(BlueprintCallable)
	void SendOnMove();
};
