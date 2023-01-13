// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleWidget.h"
#include "VehicleUnit.h"

int32 UVehicleWidget::GetRemainingMovement()
{
	return selectedVehicle ? selectedVehicle->RemainingMoves() : 0;
}