// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITurnBased.h"
#include "Neighbor.h"
#include "GameFramework/Actor.h"
#include "TileSystem.generated.h"

class ATileActor;


/** Instantiates and maintains the tile grid. Also provides methods of interacting with tiles in the system. */
UCLASS(notplaceable)
class INFECTIONSTRATEGY_API ATileSystem : public AActor, public ITurnBased
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ATileSystem();

	/** Sets the tile at a certain point on the grid as occupied.
	 * @param x - X coordinate of target tile
	 * @param y - Y coordinate of target tile
	 * @return True on success, false if tile not valid or already occupied
	 */
	bool OccupyTile(const int32 x, const int32 y) const;

	/** Gets the world location of a tile in the system.
	 * @param x - X coordinate of target tile
	 * @param y - Y coordinate of target tile
	 * @return World location of target tile 
	 * @warning Ensure x and y are less than GridWidth and GridHeight, respectively. 
	 */
	FVector GetLocationAtTile(const int32 x, const int32 y) const;
	
	/** Gets a neighbor of a particular tile.
	 * @param x - X coordinate of target tile
	 * @param y - Y coordinate of target tile
	 * @param neighborType - The neighbor to be found
	 * @return A pointer to the chosen neighboring tile or nullptr.
	 */
	ATileActor* GetNeighbor(const int32 x, const int32 y, const ENeighbor neighborType);

	/* TurnBased Interface*/
	virtual void OnTurnBegin(const int32 playerId) override;
	virtual void OnTurnEnd(const int32 playerId) override;
	/* End TurnBased Interface */

	/** Height of the tile grid */
	UPROPERTY(EditInstanceOnly)
	uint8 GridHeight = 25;

	/* Width of the tile grid */
	UPROPERTY(EditInstanceOnly)
	uint8 GridWidth = 25;

	/* Width of the tiles used by the system, in cm */
	UPROPERTY(EditInstanceOnly)
	float TileWidth = 100.0f;

protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Sets a tile as volatile, meaning it needs to be updated each turn.
	 * @param tile - Tile to be marked
	 */
	void MarkTileAsVolatile(ATileActor* tile);

	/** The array of tiles currrently marked as volatile */
	UPROPERTY()
	TArray<ATileActor*> VolatileTiles;

private:
	/** The tile grid containing all tiles in the system */
	TArray<TArray<ATileActor*>> TileGrid;

	/** Template from which all tiles are created */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATileActor> TileTemplate;
};
