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
	// Sets default values for this actor's properties.
	ATileActor();

	/** Stores the tiles system coordinates of the tile internally.
	 * @param x - X coordinate
	 * @param y - Y coordinate
	 */
	void SetCoordinates(const int32 x, const int32 y);

	/** Informs tile that it is selected for a movement preview */
	bool SelectForMovement();

	/** Informs tile it is being targeted. */
	void SelectForTargeting();

	/** Is the tile available to be selected? */
	bool CanSelect() const;

	/** Deselect the tile. */
	void Deselect();

	/** Updates the tile's gas level based on its neighbors. */
	void RecalculateGasLevel();

	/** Updates whether the tile is connected to the gas supply. */
	void RecalculateIsConnected();

	/** Determines whether the tile should be updated on EndTurn or not. */
	bool IsVolatile() const;

	/** Find a tile underneath an arbitrary location.
	 * @param location - Location to be tested
	 * @return Point to ATileActor under the location or nullptr if none */
	static ATileActor* GetTileUnderLocation(const FVector& location);

	/* Targetable Interface */
	void Target() override;
	void Untarget() override;
	/* End Targetable interface */
	
	/* TurnBased Interface */
	virtual void OnTurnBegin(int32 playerId) override;
	virtual void OnTurnEnd(int32 playerId) override;
	/* End TurnBased Interface */

	/** Array of adjacent tiles */
	UPROPERTY(VisibleAnywhere)
	TArray<ATileActor*> Neighbors;

	/** Selection state machine */
	UPROPERTY(VisibleAnywhere)
	USelectionStateComponent* SelectionState;

	/** Gas component for gas behaviors */
	UPROPERTY(VisibleAnywhere)
	UGasComponent* Gas;

	/** Delegate that executes when the tile enters a volatile state */
	FOnVolatileChangedDelegate OnVolatileStateBegin;

	/** Delegate that executes when the tile exits a volatile state */
	FOnVolatileChangedDelegate OnVolatileStateEnd;

	/* Is the tile occupied by an object? */
	bool bOccupied = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Base mesh for the tile */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TileMesh;

	/** Mesh of the UI that indicates when the tile is selected */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* IndicatorMesh;

	/** Material to be used when the tile is selected for movement */
	UPROPERTY(EditAnywhere)
	UMaterialInstance* MovementStateMaterial;

	/** Material to be used when the tile is selected for targeting */
	UPROPERTY(EditAnywhere)
	UMaterialInstance* TargetingStateMaterial;

	/** X coordinate of the tile on the TileSystem */
	int32 XCoord = 0;

	/** Y coordinate of the tile on the TileSystem */
	int32 YCoord = 0;
};
