// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionStateComponent.h"

// Sets default values for this component's properties
USelectionStateComponent::USelectionStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USelectionStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USelectionStateComponent::ClearStates()
{
	bMovement = false;
	bTargeting = false;
}

void USelectionStateComponent::SelectDefaultState()
{
	if (bMovement || bTargeting)
	{
		ClearStates();

		OnDefaultState.ExecuteIfBound();
	}
}

void USelectionStateComponent::SelectForMovement()
{
	ClearStates();

	bMovement = true;
	OnMovementSelected.ExecuteIfBound();
}

void USelectionStateComponent::SelectForTargeting()
{
	ClearStates();

	bTargeting = true;
	OnTargetingSelected.ExecuteIfBound();
}

bool USelectionStateComponent::MovementSelected()
{
	return bMovement;
}

bool USelectionStateComponent::TargetingSelected()
{
	return bTargeting;
}