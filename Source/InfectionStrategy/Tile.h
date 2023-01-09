// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tile.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTIONSTRATEGY_API UTile : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTile();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
};
