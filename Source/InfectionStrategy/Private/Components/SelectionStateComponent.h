// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionStateComponent.generated.h"

DECLARE_DELEGATE(FOnMovementSelected);
DECLARE_DELEGATE(FOnTargetingSelected);
DECLARE_DELEGATE(FOnDefaultState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTIONSTRATEGY_API USelectionStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USelectionStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	//visual states
	bool bTargeting = false;

	bool bMovement = false;

	void ClearStates();
public:
	void SelectDefaultState();

	void SelectForMovement();
	void SelectForTargeting();

	bool MovementSelected();
	bool TargetingSelected();
	
	FOnMovementSelected OnMovementSelected;
	FOnTargetingSelected OnTargetingSelected;
	FOnDefaultState OnDefaultState;
};
