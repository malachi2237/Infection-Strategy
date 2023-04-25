// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleWidget.h"
#include "VehicleUnit.h"

int32 UVehicleWidget::GetRemainingMovement() const
{
	return SelectedVehicle ? SelectedVehicle->RemainingMoves() : 0;
}

void UVehicleWidget::SendOnMovementSelected()
{
	OnMovementSelected.ExecuteIfBound();
}

void UVehicleWidget::SendOnAttackSelected()
{
	OnAttackSelected.ExecuteIfBound();
}
void UVehicleWidget::SendOnDeselect()
{
	OnDeselect.ExecuteIfBound();
}

void UVehicleWidget::SendOnAttack()
{
	OnAttack.ExecuteIfBound();
}

void UVehicleWidget::SendOnMove()
{
	OnMove.ExecuteIfBound();
}

void UVehicleWidget::Deselect_Implementation() { }