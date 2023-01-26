// Fill out your copyright notice in the Description page of Project Settings.


#include "TileActor.h"

#include "SelectionStateComponent.h"
#include "GasComponent.h"


ATileActor::ATileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	SetRootComponent(TileMesh);
	TileMesh->SetMobility(EComponentMobility::Stationary);

	IndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Indicator Mesh"));
	IndicatorMesh->SetupAttachment(RootComponent);
	IndicatorMesh->SetMobility(EComponentMobility::Stationary);
	
	IndicatorMesh->SetVisibility(false);
	IndicatorMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

	if (CubeVisualAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(CubeVisualAsset.Object);
		TileMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		IndicatorMesh->SetStaticMesh(CubeVisualAsset.Object);
		IndicatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	}

	SelectionState = CreateDefaultSubobject<USelectionStateComponent>(TEXT("SelectionState"));
	Gas = CreateDefaultSubobject<UGasComponent>(TEXT("Gas"));

	Neighbors.Init(nullptr, 8);
}


void ATileActor::BeginPlay()
{
	Super::BeginPlay();
	
	SelectionState->OnDefaultState.BindLambda([this] {IndicatorMesh->SetVisibility(false); });
	SelectionState->OnMovementSelected.BindLambda([this]
		{
			IndicatorMesh->SetMaterial(0, MovementStateMaterial);
			IndicatorMesh->SetVisibility(true);
		});

	SelectionState->OnTargetingSelected.BindLambda([this]
		{
			IndicatorMesh->SetMaterial(0, TargetingStateMaterial);
			IndicatorMesh->SetVisibility(true);
		});
}

void ATileActor::SetCoordinates(const int32 x, const int32 y)
{
	XCoord = x;
	YCoord = y;
}

bool ATileActor::SelectForMovement()
{
	if (bOccupied || SelectionState->MovementSelected())
		return false;

	SelectionState->SelectForMovement();

	return true;
}

bool ATileActor::CanSelect() const
{
	return !(bOccupied || SelectionState->MovementSelected());
}

void ATileActor::SelectForTargeting()
{
	SelectionState->SelectForTargeting();
}

void ATileActor::Deselect()
{
	SelectionState->SelectDefaultState();
}

void ATileActor::RecalculateGasLevel()
{
	bool newLevel = false;

	int gasLevel = 0;
	for (int i = 0; i < 8; i++)
	{
		if (Neighbors[i] != nullptr)
		{
			if (newLevel)
			{
				gasLevel++;
				newLevel = false;
			}

			newLevel = true;
		}
	}

	Gas->SetGasLevel(gasLevel);
}

void ATileActor::RecalculateIsConnected()
{
	if (Gas->GetGasLevel())
	{
		for (int i = 0; i < 8; i++)
		{
			if (Neighbors[i] != nullptr && Neighbors[i]->Gas->IsConnected())
			{
				Gas->SetConnected(true);
				if (IsVolatile())
					OnVolatileStateBegin.ExecuteIfBound(this);
				return;
			}
		}
	}

	Gas->SetConnected(false);
}

ATileActor* ATileActor::GetTileUnderLocation(const FVector &location)
{
	FHitResult hit;
	FVector end = location + FVector::DownVector * 500.0;

	FCollisionObjectQueryParams params;

	if (GEngine && GEngine->GameViewport->GetWorld()->LineTraceSingleByObjectType(hit, location, end, params))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, hit.GetActor()->GetName());

		if (ATileActor* floor = Cast<ATileActor>(hit.GetActor()))
			return floor;
	}

	return nullptr;
}

bool ATileActor::IsVolatile() const
{
	return Gas->GetGasLevel() && Gas->IsConnected();
}

void ATileActor::Target()
{

}

void ATileActor::Untarget()
{

}

void ATileActor::OnTurnBegin(int32 playerId)
{

}

void ATileActor::OnTurnEnd(int32 playerId)
{

}