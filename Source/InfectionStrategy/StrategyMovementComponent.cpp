// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyMovementComponent.h"

// Sets default values for this component's properties
UStrategyMovementComponent::UStrategyMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStrategyMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStrategyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector inputVector = FVector::ZeroVector;

	inputVector = ConsumeInputVector();
	GetPawnOwner()->AddActorWorldOffset(inputVector * moveSpeed);// *DeltaTime);
	// ...
}

