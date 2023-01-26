// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleUnit.h"

#include "TileActor.h"
#include "SelectionStateComponent.h"
#include "TileMovementComponent.h"

// Sets default values
AVehicleUnit::AVehicleUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	SetRootComponent(mesh);
	mesh->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded())
	{
		mesh->SetStaticMesh(CubeVisualAsset.Object);
		mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> SelectionMaterialVisualAsset(TEXT("/Game/Materials/MI_TileSelected.MI_TileSelected"));

	if (SelectionMaterialVisualAsset.Succeeded())
		selectedStateMaterial = SelectionMaterialVisualAsset.Object;

	mesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));

	selectionState = CreateDefaultSubobject<USelectionStateComponent>(TEXT("Selection State"));
	tileMovement = CreateDefaultSubobject<UTileMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AVehicleUnit::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	UMaterialInterface* defaultMaterial = mesh->GetMaterial(0);
	selectionState->OnMovementSelected.BindUObject(this, &AVehicleUnit::OnSelection);

	if (defaultMaterial != nullptr)
	{
		selectionState->OnDefaultState.BindLambda([this, defaultMaterial]
			{mesh->SetMaterial(0, defaultMaterial); });
	}

	tileMovement->OnMovementComplete.AddDynamic(this, &AVehicleUnit::OnMovementComplete);
}

// Called every frame
void AVehicleUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*MoveTo*/
}

void AVehicleUnit::OnMovementComplete()
{
	tile = ATileActor::GetTileUnderLocation(GetActorLocation());
}

void AVehicleUnit::OnSelection()
{
	mesh->SetMaterial(0, selectedStateMaterial);
	tile = ATileActor::GetTileUnderLocation(GetActorLocation());
}

bool AVehicleUnit::TrySelect(int32 playerId)
{
	if ((playerId == owner) && (CanMove() || bCanAttack))
	{
		selectionState->SelectForMovement();
		return true;
	}

	return false;
}

void AVehicleUnit::Deselect()
{
	selectionState->SelectDefaultState();
}

bool AVehicleUnit::CanMove()
{
	return currentMovement < maxMovement;
}

int32 AVehicleUnit::RemainingMoves()
{
	return maxMovement - currentMovement;
}

bool AVehicleUnit::TryMovement()
{
	if (CanMove())
	{
		currentMovement++;
		return true;
	}

	return false;
}

void AVehicleUnit::UndoMovement(int32 steps)
{
	currentMovement = FMath::Clamp(currentMovement - steps, 0, maxMovement);
}

void AVehicleUnit::SetPlayerOwner(int32 playerId)
{
	owner = playerId;
}

void AVehicleUnit::Target()
{

}

void AVehicleUnit::Untarget()
{

}

void AVehicleUnit::OnTurnBegin(int32 playerId)
{

}

void AVehicleUnit::OnTurnEnd(int32 playerId)
{
	currentMovement = 0;
	bCanAttack = true;
}

UPawnMovementComponent* AVehicleUnit::GetMovementComponent() const { return tileMovement; };