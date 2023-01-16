// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITargetable.h"
#include "ITurnBased.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

class USelectionStateComponent;
class UGasComponent;

DECLARE_DELEGATE_OneParam(FOnVolatileChangedDelegate, ATileActor*);

UCLASS()
class INFECTIONSTRATEGY_API ATileActor : public AActor, public ITargetable, public ITurnBased
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

	FOnVolatileChangedDelegate OnVolatileStateBegin;

	FOnVolatileChangedDelegate OnVolatileStateEnd;

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

	bool CanSelect() const;
	void Deselect();

	void RecalculateGasLevel();
	void RecalculateIsConnected();

	bool IsVolatile() const;
	static ATileActor* GetTileUnderLocation(const FVector &location);

	/* Targetable Interface */
	void Target() override;
	void Untarget() override;

	/* TurnBased Interface*/
	virtual void OnTurnBegin(int32 playerId) override;
	virtual void OnTurnEnd(int32 playerId) override;
};
