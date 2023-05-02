// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSystem.h"
#include "TileActor.h"
#include "InfectionStrategyGameMode.h"

// Sets default values for this component's properties
ATileSystem::ATileSystem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileSystem::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	if (GetWorld()->IsGameWorld())
	{
		float tileScale = TileWidth / 100.0f;
		int xOffset = -GridWidth / 2;
		int yOffset = GridHeight / 2;

		if (!TileTemplate)
			TileTemplate = ATileActor::StaticClass();

		for (int x = 0; x < GridHeight; x++)
		{
			TileGrid.Add(TArray<ATileActor*>());

			for (int y = 0; y < GridWidth; y++)
			{
				FVector const* spawnLocation = new FVector((yOffset - y) * TileWidth, (x + xOffset) * TileWidth, 0.0f);

				auto newTileActor = (ATileActor*)GetWorld()->SpawnActor(TileTemplate, spawnLocation);

				check(newTileActor);

				/*newTileActor->SetCoordinates(x, y);
				newTileActor->OnVolatileStateBegin.BindUObject(this, &ATileSystem::MarkTileAsVolatile);*/
				TileGrid[x].Add(newTileActor);
			}
		}

		//for (int x = 0; x < GridWidth; x++)
		//{
		//	for (int y = 0; y < GridHeight; y++)
		//	{
		//		for (int i = 0; i < 8; i++)
		//		{
		//			TileGrid[x][y]->Neighbors[i] = GetNeighbor(x, y, (ENeighbor)i);
		//		}
		//	}
		//}
	}
}
void ATileSystem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld())
	{
		for (int x = 0; x < GridWidth; x++)
		{
			for (int y = 0; y < GridHeight; y++)
			{
				TileGrid[x][y]->SetCoordinates(x, y);
				TileGrid[x][y]->OnVolatileStateBegin.BindUObject(this, &ATileSystem::MarkTileAsVolatile);

				for (int i = 0; i < 8; i++)
				{
					TileGrid[x][y]->Neighbors[i] = GetNeighbor(x, y, (ENeighbor)i);


				}
			}
		}
	}
}

// Called when the game starts
void ATileSystem::BeginPlay()
{
	Super::BeginPlay();

	
	/*float tileScale = TileWidth / 100.0f;
	int xOffset = -GridWidth / 2;
	int yOffset = GridHeight / 2;

	for (int x = 0; x < GridHeight; x++)
	{
		TileGrid.Add(TArray<ATileActor*>());

		for (int y = 0; y < GridWidth; y++)
		{
			FVector const *spawnLocation = new FVector((yOffset - y) * TileWidth, (x + xOffset) * TileWidth, 0.0f);

			auto newTileActor = (ATileActor*)GetWorld()->SpawnActor(TileTemplate, spawnLocation);

			newTileActor->SetCoordinates(x, y);
			newTileActor->OnVolatileStateBegin.BindUObject(this, &ATileSystem::MarkTileAsVolatile);
			TileGrid[x].Add(newTileActor);
		}
	}

	for (int x = 0; x < GridWidth; x++)
	{
		for (int y = 0; y < GridHeight; y++)
		{
			for (int i = 0; i < 8; i++)
			{
				TileGrid[x][y]->Neighbors[i] = GetNeighbor(x, y, (ENeighbor)i);
			}
		}
	}*/
}

ATileActor* ATileSystem::GetNeighbor(const int32 x, const int32 y, const ENeighbor neighborType)
{
	ATileActor* selectedNeighbor = nullptr;

	switch (neighborType)
	{
	case ENeighbor::Up:
		if (y > 0)
			selectedNeighbor = TileGrid[x][y - 1];
		break;
	case ENeighbor::Down:
		if (y + 1 < GridHeight)
			selectedNeighbor = TileGrid[x][y + 1];
		break;
	case ENeighbor::Left:
		if (x > 0)
			selectedNeighbor = TileGrid[x - 1][y];
		break;
	case ENeighbor::Right:
		if (x + 1 < GridWidth)
			selectedNeighbor = TileGrid[x + 1][y];
		break;
	case ENeighbor::TopLeft:
		if (y > 0 && x > 0)
			selectedNeighbor = TileGrid[x - 1][y - 1];
		break;
	case ENeighbor::TopRight:
		if (y > 0 && x + 1 < GridWidth)
			selectedNeighbor = TileGrid[x + 1][y - 1];
		break;
	case ENeighbor::BottomLeft:
		if (y + 1 < GridHeight && x > 0)
			selectedNeighbor = TileGrid[x - 1][y + 1];
		break;
	case ENeighbor::BottomRight:
		if (y + 1 < GridHeight && x + 1< GridWidth)
			selectedNeighbor = TileGrid[x + 1][y + 1];
	}

	return selectedNeighbor;
}

bool ATileSystem::OccupyTile(const int32 x, const int32 y) const
{
	if (x < GridWidth && y < GridHeight)
	{
		ATileActor* tile = TileGrid[x][y];

		return tile->bOccupied != (tile->bOccupied = true);
	}

	return false;
}

FVector ATileSystem::GetLocationAtTile(const int32 x, const int32 y) const
{
	check(x < GridWidth);
	check(y < GridHeight);

	return TileGrid[x][y]->GetActorLocation();
}

void ATileSystem::MarkTileAsVolatile(ATileActor* tile)
{
	VolatileTiles.Add(tile);
}

void ATileSystem::OnTurnBegin(const int32 playerId)
{
	VolatileTiles = VolatileTiles.FilterByPredicate([](ATileActor* tile) { return tile->IsVolatile(); });

	for (auto& tile : VolatileTiles)
		tile->OnTurnBegin(playerId);
}

void ATileSystem::OnTurnEnd(const int32 playerId)
{
	VolatileTiles = VolatileTiles.FilterByPredicate([](ATileActor* tile) { return tile->IsVolatile(); });

	for (auto& tile : VolatileTiles)
		tile->OnTurnEnd(playerId);
}