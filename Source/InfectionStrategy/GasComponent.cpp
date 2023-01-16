// Fill out your copyright notice in the Description page of Project Settings.


#include "GasComponent.h"

// Sets default values for this component's properties
UGasComponent::UGasComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGasComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UGasComponent::IsConnected()
{
	return bConnected;
}

void UGasComponent::SetConnected(bool isConnected)
{
	bConnected = isConnected && gasLevel;
}



void UGasComponent::SetGasLevel(int32 newLevel)
{
	gasLevel = FMath::Clamp(newLevel, 0, 4);
}

int32 UGasComponent::GetGasLevel() const
{
	return gasLevel;
}