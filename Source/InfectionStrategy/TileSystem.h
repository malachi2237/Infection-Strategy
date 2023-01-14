// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileSystem.generated.h"

class ATileActor;

UENUM()
enum class Neighbor
{
	Up,
	Down,
	Left,
	Right,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTIONSTRATEGY_API UTileSystem : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<TArray<ATileActor*>> tileGrid;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATileActor> tileTemplate;
public:	
	// Sets default values for this component's properties
	UTileSystem();

	UPROPERTY(EditInstanceOnly)
	uint8 gridHeight = 50;

	UPROPERTY(EditInstanceOnly)
	uint8 gridWidth = 50;

	UPROPERTY(EditInstanceOnly)
	float tileWidth = 100.0f;

	ATileActor* GetNeighbor(int32 x, int32 y, Neighbor neighborType);

	bool OccupyTile(int32 x, int32 y);

	FVector GetLocationAt(int32 x, int32 y);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
		
};
