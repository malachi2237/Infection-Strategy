// Fill out your copyright notice in the Description page of Project Settings.


#include "TileActor.h"

#include "SelectionStateComponent.h"
#include "GasComponent.h"
// Sets default values
ATileActor::ATileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	SetRootComponent(tileMesh);
	tileMesh->SetMobility(EComponentMobility::Stationary);

	indicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Indicator Mesh"));
	indicatorMesh->SetupAttachment(RootComponent);
	indicatorMesh->SetMobility(EComponentMobility::Stationary);
	
	indicatorMesh->SetVisibility(false);
	indicatorMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

	if (CubeVisualAsset.Succeeded())
	{
		tileMesh->SetStaticMesh(CubeVisualAsset.Object);
		tileMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		indicatorMesh->SetStaticMesh(CubeVisualAsset.Object);
		indicatorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	}

	selectionState = CreateDefaultSubobject<USelectionStateComponent>(TEXT("SelectionState"));
	gas = CreateDefaultSubobject<UGasComponent>(TEXT("Gas"));

	neighbors.Init(nullptr, 8);
}

void ATileActor::SetCoordinates(int32 x, int32 y)
{
	xCoord = x;
	yCoord = y;
}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();
	
	selectionState->OnDefaultState.BindLambda([this] {indicatorMesh->SetVisibility(false); });
	selectionState->OnMovementSelected.BindLambda([this]
		{
			indicatorMesh->SetMaterial(0, movementStateMaterial);
			indicatorMesh->SetVisibility(true);
		});

	selectionState->OnTargetingSelected.BindLambda([this]
		{
			indicatorMesh->SetMaterial(0, targetingStateMaterial);
			indicatorMesh->SetVisibility(true);
		});
}

bool ATileActor::SelectForMovement()
{
	if (bOccupied || selectionState->MovementSelected())
		return false;

	selectionState->SelectForMovement();

	return true;
}

bool ATileActor::CanSelect()
{
	return !(bOccupied || selectionState->MovementSelected());
}

void ATileActor::SelectForTargeting()
{
	selectionState->SelectForTargeting();
}

void ATileActor::Deselect()
{
	selectionState->SelectDefaultState();
}

void ATileActor::RecalculateGasLevel()
{
	bool newLevel = false;

	int gasLevel = 0;
	for (int i = 0; i < 8; i++)
	{
		if (neighbors[i] != nullptr)
		{
			if (newLevel)
			{
				gasLevel++;
				newLevel = false;
			}

			newLevel = true;
		}
	}

	gas->SetGasLevel(gasLevel);
}

void ATileActor::RecalculateIsConnected()
{
	for (int i = 0; i < 8; i++)
	{
		if (neighbors[i] != nullptr && neighbors[i]->gas->IsConnected())
		{
			gas->SetConnected(true);
			return;
		}
	}
	
	gas->SetConnected(false);
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