// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSystem.h"
#include "TileActor.h"
#include "InfectionStrategyGameMode.h"

// Sets default values for this component's properties
UTileSystem::UTileSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTileSystem::BeginPlay()
{
	Super::BeginPlay();

	float tileScale = tileWidth / 100.0f;
	int xOffset = -gridWidth / 2;
	int yOffset = gridHeight / 2;

	StaticCast<AInfectionStrategyGameMode *>(GetWorld()->GetAuthGameMode())->TileSystem = this;

	for (int x = 0; x < gridHeight; x++)
	{
		tileGrid.Add(TArray<ATileActor*>());

		for (int y = 0; y < gridWidth; y++)
		{
			FVector const *spawnLocation = new FVector((yOffset - y) * tileWidth, (x + xOffset) * tileWidth, 0.0f);

			auto newTileActor = (ATileActor*)GetWorld()->SpawnActor(tileTemplate, spawnLocation);

			newTileActor->SetCoordinates(x, y);
			newTileActor->OnVolatileStateBegin.BindUObject(this, &UTileSystem::MarkTileAsVolatile);
			tileGrid[x].Add(newTileActor);
		}
	}

	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			for (int i = 0; i < 8; i++)
			{
				tileGrid[x][y]->Neighbors[i] = GetNeighbor(x, y, (Neighbor)i);
			}
		}
	}
}

ATileActor* UTileSystem::GetNeighbor(int32 x, int32 y, Neighbor neighborType)
{
	ATileActor* selectedNeighbor = nullptr;

	switch (neighborType)
	{
	case Neighbor::Up:
		if (y > 0)
			selectedNeighbor = tileGrid[x][y - 1];
		break;
	case Neighbor::Down:
		if (y + 1 < gridHeight)
			selectedNeighbor = tileGrid[x][y + 1];
		break;
	case Neighbor::Left:
		if (x > 0)
			selectedNeighbor = tileGrid[x - 1][y];
		break;
	case Neighbor::Right:
		if (x + 1 < gridWidth)
			selectedNeighbor = tileGrid[x + 1][y];
		break;
	case Neighbor::TopLeft:
		if (y > 0 && x > 0)
			selectedNeighbor = tileGrid[x - 1][y - 1];
		break;
	case Neighbor::TopRight:
		if (y > 0 && x + 1 < gridWidth)
			selectedNeighbor = tileGrid[x + 1][y - 1];
		break;
	case Neighbor::BottomLeft:
		if (y + 1 < gridHeight && x > 0)
			selectedNeighbor = tileGrid[x - 1][y + 1];
		break;
	case Neighbor::BottomRight:
		if (y + 1 < gridHeight && x + 1< gridWidth)
			selectedNeighbor = tileGrid[x + 1][y + 1];
	}

	return selectedNeighbor;
}

bool UTileSystem::OccupyTile(int32 x, int32 y)
{
	ATileActor* tile = tileGrid[x][y];

	//return false if tile was already occupied
	return tile->bOccupied != (tile->bOccupied = true);
}

FVector UTileSystem::GetLocationAt(int32 x, int32 y)
{
	return tileGrid[x][y]->GetActorLocation();
}

void UTileSystem::MarkTileAsVolatile(ATileActor* tile)
{
	VolatileTiles.Add(tile);
}

void UTileSystem::OnTurnBegin(int32 playerId)
{
	VolatileTiles = VolatileTiles.FilterByPredicate([](ATileActor* tile) { return tile->IsVolatile(); });

	for (auto& tile : VolatileTiles)
		tile->OnTurnBegin(playerId);
}

void UTileSystem::OnTurnEnd(int32 playerId)
{
	VolatileTiles = VolatileTiles.FilterByPredicate([](ATileActor* tile) { return tile->IsVolatile(); });

	for (auto& tile : VolatileTiles)
		tile->OnTurnEnd(playerId);
}