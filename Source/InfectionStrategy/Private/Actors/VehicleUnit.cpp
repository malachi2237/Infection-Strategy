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

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded())
	{
		Mesh->SetStaticMesh(CubeVisualAsset.Object);
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> SelectionMaterialVisualAsset(TEXT("/Game/Materials/MI_TileSelected.MI_TileSelected"));

	if (SelectionMaterialVisualAsset.Succeeded())
		SelectedStateMaterial = SelectionMaterialVisualAsset.Object;

	Mesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));

	SelectionState = CreateDefaultSubobject<USelectionStateComponent>(TEXT("Selection State"));
	TileMovement = CreateDefaultSubobject<UTileMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AVehicleUnit::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	UMaterialInterface* defaultMaterial = Mesh->GetMaterial(0);
	SelectionState->OnMovementSelected.BindUObject(this, &AVehicleUnit::OnSelection);

	if (defaultMaterial != nullptr)
	{
		SelectionState->OnDefaultState.BindLambda([this, defaultMaterial]
			{Mesh->SetMaterial(0, defaultMaterial); });
	}

	TileMovement->OnMovementComplete.AddDynamic(this, &AVehicleUnit::OnMovementComplete);
}

// Called every frame
void AVehicleUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*MoveTo*/
}

void AVehicleUnit::OnMovementComplete()
{
	Tile = ATileActor::GetTileUnderLocation(GetActorLocation());
}

void AVehicleUnit::OnSelection()
{
	Mesh->SetMaterial(0, SelectedStateMaterial);
	Tile = ATileActor::GetTileUnderLocation(GetActorLocation());
}

bool AVehicleUnit::TrySelect(int32 playerId)
{
	if ((playerId == Owner) && (CanMove() || bCanAttack))
	{
		SelectionState->SelectForMovement();
		return true;
	}

	return false;
}

void AVehicleUnit::Deselect()
{
	SelectionState->SelectDefaultState();
}

bool AVehicleUnit::CanMove()
{
	return CurrentMovement < MaxMovement;
}

int32 AVehicleUnit::RemainingMoves()
{
	return MaxMovement - CurrentMovement;
}

bool AVehicleUnit::TryMovement()
{
	if (CanMove())
	{
		CurrentMovement++;
		return true;
	}

	return false;
}

void AVehicleUnit::UndoMovement(int32 steps)
{
	CurrentMovement = FMath::Clamp(CurrentMovement - steps, 0, MaxMovement);
}

void AVehicleUnit::SetPlayerOwner(int32 playerId)
{
	Owner = playerId;
}

void AVehicleUnit::Target()
{

}

void AVehicleUnit::Untarget()
{

}

void AVehicleUnit::OnTurnBegin(const int32 playerId)
{

}

void AVehicleUnit::OnTurnEnd(const int32 playerId)
{
	CurrentMovement = 0;
	bCanAttack = true;
}

UPawnMovementComponent* AVehicleUnit::GetMovementComponent() const { return TileMovement; };