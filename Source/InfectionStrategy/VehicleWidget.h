// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleWidget.generated.h"


class AVehicleUnit;

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

protected:
	AVehicleUnit* selectedVehicle;

	void OnDeselect();
	void OnAttack();
	void OnMove();
};
