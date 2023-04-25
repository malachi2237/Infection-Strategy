// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectionStrategyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/GameEngine.h"
#include "InfectionStrategyCharacter.h"
#include "Engine/World.h"
#include "Widgets/GameplayHUD.h"
#include "VehicleWidget.h"
#include "TileSystem.h"
#include "TileMovementComponent.h"
#include "VehicleUnit.h"
#include "TileActor.h"

#include <Kismet/GameplayStatics.h>

#define MAX_UNITS 5

DECLARE_DELEGATE_OneParam(FTryTileMovementDelegate, ENeighbor);
DECLARE_DELEGATE_OneParam(FCameraMovementDelegate, int32);

AInfectionStrategyPlayerController::AInfectionStrategyPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AInfectionStrategyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	/* Setup the hud */
	if (AGameplayHUD* hud = GetHUD<AGameplayHUD>())
	{
		/* If this is player 1, then load the HUD. */
		if (auto firstPlayerController = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			if (firstPlayerController == this)
			{
				hud->AssignPlayer(0);
				hud->bDisplayOnBeginPlay = true;
			}
			else
				hud->AssignPlayer(1);
		}

		if (!hud->HasActorBegunPlay())
			hud->DispatchBeginPlay();

		if (hud->VehicleHudInstance)
		{
			hud->VehicleHudInstance->OnMovementSelected.BindLambda([this] {
				bIsMovingUnit = true; bIsTargeting = false; });
			hud->VehicleHudInstance->OnAttackSelected.BindLambda([this] {
				bIsMovingUnit = false; bIsTargeting = true; });

			hud->VehicleHudInstance->OnDeselect.BindLambda([this] { SelectUnit(nullptr); });
			hud->VehicleHudInstance->OnMove.BindUObject(this, &AInfectionStrategyPlayerController::OnConfirmMoveReleased);
		}
	}
}

void AInfectionStrategyPlayerController::OnTurnBegin(const int32 player)
{
	if (AGameplayHUD* hud = GetHUD<AGameplayHUD>())
		hud->OnTurnBegin(player);
}

void AInfectionStrategyPlayerController::OnTurnEnd(const int32 player)
{
	SelectUnit(nullptr);

	if (AGameplayHUD* hud = GetHUD<AGameplayHUD>())
		hud->OnTurnEnd(player);
}

void AInfectionStrategyPlayerController::ClaimUnit(AVehicleUnit* unit, const int32 ownerId)
{
	unit->SetPlayerOwner(ownerId);
	if (auto tileMoveComp = Cast<UTileMovementComponent>(unit->GetMovementComponent()))
	{
		//tileMoveComp->OnMovementComplete.Add()
	}
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

void AInfectionStrategyPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Released, this, &AInfectionStrategyPlayerController::OnSelectUnitReleased);

	InputComponent->BindAction<FTryTileMovementDelegate>("MoveUp", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, ENeighbor::Up);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveDown", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, ENeighbor::Down);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveLeft", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, ENeighbor::Left);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveRight", IE_Released, this, &AInfectionStrategyPlayerController::TryTileMovement, ENeighbor::Right);

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

void AInfectionStrategyPlayerController::TryTileMovement(ENeighbor direction)
{
	ATileActor* currentTile = nullptr;
	ATileActor* nextTile = nullptr;

	if (SelectedVehicle && bIsMovingUnit)
	{
		if (!MovementQueue.IsEmpty())
		{
			currentTile = MovementQueue.Last();
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tile from Deque"));
		}
		else if (SelectedVehicle->Tile)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tile from vehicle"));
			currentTile = SelectedVehicle->Tile;
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Tile found"));
			return;
		}

		nextTile = currentTile->Neighbors[static_cast<int>(direction)];

		if ((nextTile && nextTile->CanSelect()) && SelectedVehicle->TryMovement())
		{
			nextTile->SelectForMovement();
			MovementQueue.PushLast(nextTile);
		}
	}
}

void AInfectionStrategyPlayerController::UndoMovement()
{
	ATileActor* lastTile;

	if (MovementQueue.TryPopLast(lastTile))
		lastTile->Deselect();

	if (SelectedVehicle)
		SelectedVehicle->UndoMovement(1);
}

void AInfectionStrategyPlayerController::UndoAllMovement()
{
	ATileActor* lastTile;
	int moveCount = MovementQueue.Num();

	while (!MovementQueue.IsEmpty())
	{
		if (MovementQueue.TryPopLast(lastTile))
			lastTile->Deselect();
	}

	if (SelectedVehicle)
		SelectedVehicle->UndoMovement(moveCount);
}

void AInfectionStrategyPlayerController::SelectUnit(AVehicleUnit *unit)
{
	if (SelectedVehicle)
		SelectedVehicle->Deselect();

	UndoAllMovement();

	bIsMovingUnit = false;
	bIsTargeting = false;

	if (unit && unit->TrySelect(playerId))
	{
		if (AGameplayHUD* hud = GetHUD<AGameplayHUD>())
		{
			hud->SelectVehicle(unit);
		}
		
		SelectedVehicle = unit;
		
		return;
	}

	SelectedVehicle = nullptr;

	if (AGameplayHUD* hud = GetHUD<AGameplayHUD>())
	{
		hud->DeselectVehicle();
	}
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

void AInfectionStrategyPlayerController::OnConfirmMoveReleased()
{
	if (SelectedVehicle)
	{
		ATileActor* currentTile = SelectedVehicle->Tile;
		FVector previousLocation = SelectedVehicle->GetActorLocation();

		currentTile->bOccupied = false;

		while (MovementQueue.TryPopFirst(currentTile))
		{
			FVector direction = currentTile->GetActorLocation() - previousLocation;
			direction.Z += 30.f;
			direction.Normalize();
			SelectedVehicle->AddMovementInput(direction, FVector::Dist(previousLocation, currentTile->GetActorLocation()));

			currentTile->Deselect();
			previousLocation = currentTile->GetActorLocation();
		}

		currentTile->bOccupied = true;
		bIsMovingUnit = false;
	}
}

void AInfectionStrategyPlayerController::OnMoveCameraVerticalPressed(const int32 direction)
{
	cameraMoveVert = FMath::Clamp(direction, -1, 1);
}

void AInfectionStrategyPlayerController::OnMoveCameraHorizontalPressed(const int32 direction)
{
	cameraMoveHoriz = FMath::Clamp(direction, -1, 1);
}

void AInfectionStrategyPlayerController::OnMoveCameraVerticalReleased()
{
	cameraMoveVert = 0;
}

void AInfectionStrategyPlayerController::OnMoveCameraHorizontalReleased()
{
	cameraMoveHoriz = 0;
}
