// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GasComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTIONSTRATEGY_API UGasComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGasComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Gas States
	bool bConnected = false;

	int32 gasLevel = 0;

	int32 playerGas = -1;


public:	
	bool IsConnected();

	void SetConnected(bool isConnected);
	void SetGasLevel(int32 newLevel);
	int32 GetGasLevel() const;
};
