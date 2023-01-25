// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectionStrategyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/GameEngine.h"
#include "InfectionStrategyCharacter.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "VehicleWidget.h"
#include "TileSystem.h"
#include "VehicleUnit.h"
#include "TileActor.h"

#define MAX_UNITS 5

DECLARE_DELEGATE_OneParam(FTryTileMovementDelegate, Neighbor);
DECLARE_DELEGATE_OneParam(FCameraMovementDelegate, int32);

AInfectionStrategyPlayerController::AInfectionStrategyPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AInfectionStrategyPlayerController::BeginPlay()
{
	if (hudTemplate)
		hudInstance = CreateWidget(this, hudTemplate);

	hudInstance->AddToViewport();

	if (vehicleHudTemplate)
		vehicleHudInstance = CreateWidget<UVehicleWidget>(this, vehicleHudTemplate);

	if (vehicleHudInstance)
		vehicleHudInstance->OnDeselect.BindUObject(this, &AInfectionStrategyPlayerController::DeselectUnit);
}
void AInfectionStrategyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (cameraMoveVert || cameraMoveHoriz)
	{
		FVector inputVector = FVector(cameraMoveVert, cameraMoveHoriz, 0.0).GetSafeNormal();

		GetPawn()->AddMovementInput(inputVector, 1.f, false);
	}
}

void AInfectionStrategyPlayerController::DeselectUnit()
{
	SelectUnit(nullptr);
}

void AInfectionStrategyPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Released, this, &AInfectionStrategyPlayerController::OnSelectUnitReleased);

	InputComponent->BindAction<FTryTileMovementDelegate>("MoveUp", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, Neighbor::Up);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveDown", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, Neighbor::Down);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveLeft", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, Neighbor::Left);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveRight", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, Neighbor::Right);

	InputComponent->BindAction<FCameraMovementDelegate>("CameraUp", IE_Pressed, this, &AInfectionStrategyPlayerController::OnMoveCameraVerticalPressed, 1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraDown", IE_Pressed, this, &AInfectionStrategyPlayerController::OnMoveCameraVerticalPressed, -1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraLeft", IE_Pressed, this, &AInfectionStrategyPlayerController::OnMoveCameraHorizontalPressed, -1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraRight", IE_Pressed, this, &AInfectionStrategyPlayerController::OnMoveCameraHorizontalPressed, 1);

	InputComponent->BindAction("CameraUp", IE_Released, this, &AInfectionStrategyPlayerController::OnMoveCameraVerticalReleased);
	InputComponent->BindAction("CameraDown", IE_Released, this, &AInfectionStrategyPlayerController::OnMoveCameraVerticalReleased);
	InputComponent->BindAction("CameraLeft", IE_Released, this, &AInfectionStrategyPlayerController::OnMoveCameraHorizontalReleased);
	InputComponent->BindAction("CameraRight", IE_Released, this, &AInfectionStrategyPlayerController::OnMoveCameraHorizontalReleased);

	InputComponent->BindAction("UndoMove", IE_Released, this, &AInfectionStrategyPlayerController::UndoMovement);

	InputComponent->BindAction("Confirm", IE_Released, this, &AInfectionStrategyPlayerController::OnConfirmMoveReleased);

}

void AInfectionStrategyPlayerController::OnSelectUnitReleased()
{
	// Player is no longer pressing the input
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Click Released"));

	// We look for the location in the world where the player has pressed the input
	FVector HitLocation = FVector::ZeroVector;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	HitLocation = Hit.Location;

	//TODO: should check if this is a player owned vehicle
	if (AVehicleUnit* unit = Cast<AVehicleUnit>(Hit.GetActor()))
		SelectUnit(unit);

}

void AInfectionStrategyPlayerController::SelectUnit(AVehicleUnit *unit)
{
	if (selectedVehicle)
		selectedVehicle->Deselect();

	UndoMovement(movementQueue.Num());

	if (unit && unit->TrySelect(playerId))
	{
		if (!vehicleHudInstance->IsInViewport())
			vehicleHudInstance->AddToViewport();

		vehicleHudInstance->SetSelectedVehicle(unit);
		
		selectedVehicle = unit;
		
		return;
	}

	selectedVehicle = nullptr;
	if (vehicleHudInstance)
	{
		vehicleHudInstance->RemoveFromViewport();
		vehicleHudInstance->SetSelectedVehicle(selectedVehicle);
	}
}

void AInfectionStrategyPlayerController::TryTileMovement(Neighbor direction)
{
	ATileActor *currentTile = nullptr;
	ATileActor *nextTile = nullptr;

	if (selectedVehicle)
	{
		if (!movementQueue.IsEmpty())
		{
			currentTile = movementQueue.Last();
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tile from Deque"));
		}
		else if (selectedVehicle->tile)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tile from vehicle"));
			currentTile = selectedVehicle->tile;
		}		
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Tile found"));
			return;
		}

		nextTile = currentTile->neighbors[static_cast<int>(direction)];

		if ((nextTile && nextTile->CanSelect()) && selectedVehicle->TryMovement())
		{
			nextTile->SelectForMovement();
			movementQueue.PushLast(nextTile);
		}
	}
}

void AInfectionStrategyPlayerController::UndoMovement()
{
	ATileActor* lastTile;

	if (movementQueue.TryPopLast(lastTile))
		lastTile->Deselect();

	if (selectedVehicle)
		selectedVehicle->UndoMovement(1);
}

void AInfectionStrategyPlayerController::UndoMovement(int32 moveCount)
{
	ATileActor* lastTile;

	while (!movementQueue.IsEmpty())
	{
		if (movementQueue.TryPopLast(lastTile))
			lastTile->Deselect();
	}

	if (selectedVehicle)
		selectedVehicle->UndoMovement(moveCount);
}
void AInfectionStrategyPlayerController::OnConfirmMoveReleased()
{
	if (selectedVehicle)
	{
		ATileActor* currentTile = selectedVehicle->tile;
		FVector previousLocation = selectedVehicle->GetActorLocation();

		currentTile->bOccupied = false;

		while (movementQueue.TryPopFirst(currentTile))
		{
			FVector direction = currentTile->GetActorLocation() - previousLocation;
			direction.Z += 30.f;
			direction.Normalize();
			selectedVehicle->AddMovementInput(direction, FVector::Dist(previousLocation, currentTile->GetActorLocation()));

			currentTile->Deselect();
			previousLocation = currentTile->GetActorLocation();
		}

		currentTile->bOccupied = true;
	}
}

void AInfectionStrategyPlayerController::OnMoveCameraVerticalPressed(int32 direction)
{
	cameraMoveVert = direction;
}

void AInfectionStrategyPlayerController::OnMoveCameraHorizontalPressed(int32 direction)
{
	cameraMoveHoriz = direction;
}

void AInfectionStrategyPlayerController::OnMoveCameraVerticalReleased()
{
	cameraMoveVert = 0;
}

void AInfectionStrategyPlayerController::OnMoveCameraHorizontalReleased()
{
	cameraMoveHoriz = 0;
}

void AInfectionStrategyPlayerController::OnTurnBegin(int32 player)
{
	playerId = player;
}

void AInfectionStrategyPlayerController::OnTurnEnd(int32 player)
{
	SelectUnit(nullptr);
}