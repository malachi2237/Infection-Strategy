// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionStateComponent.h"
#include "GasComponent.h"
#include "ITargetable.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

UCLASS()
class INFECTIONSTRATEGY_API ATileActor : public AActor, public ITargetable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileActor();

	UPROPERTY(VisibleAnywhere)
	TArray<ATileActor*> neighbors;

	UPROPERTY(VisibleAnywhere)
	USelectionStateComponent* selectionState;

	UPROPERTY(VisibleAnywhere)
	UGasComponent* gas;

	//State Booleans
	bool bOccupied = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* tileMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* indicatorMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* movementStateMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* targetingStateMaterial;


	int32 xCoord = 0;
	int32 yCoord = 0;
public:
	void SetCoordinates(int32 x, int32 y);

	bool SelectForMovement();
	void SelectForTargeting();

	bool CanSelect();
	void Deselect();

	void RecalculateGasLevel();
	void RecalculateIsConnected();

	static ATileActor* GetTileUnderLocation(const FVector &location);

	void Target() override;
	void Untarget() override;
};
