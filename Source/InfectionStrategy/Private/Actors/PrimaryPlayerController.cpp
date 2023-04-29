// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PrimaryPlayerController.h"
#include "Widgets/GameplayHUD.h"

/* Should be able to get rid of at least the gamemode reference eventually. */
#include "InfectionStrategyGameMode.h"
#include "Kismet/GameplayStatics.h"

DECLARE_DELEGATE_OneParam(FTryTileMovementDelegate, ENeighbor);
DECLARE_DELEGATE_OneParam(FCameraMovementDelegate, int32);

void APrimaryPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Released, this, &APrimaryPlayerController::HandleSelectionInput);

	InputComponent->BindAction<FTryTileMovementDelegate>("MoveUp", IE_Released, this, &APrimaryPlayerController::HandleTileMovementInput, ENeighbor::Up);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveDown", IE_Released, this, &APrimaryPlayerController::HandleTileMovementInput, ENeighbor::Down);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveLeft", IE_Released, this, &APrimaryPlayerController::HandleTileMovementInput, ENeighbor::Left);
	InputComponent->BindAction<FTryTileMovementDelegate>("MoveRight", IE_Released, this, &APrimaryPlayerController::HandleTileMovementInput, ENeighbor::Right);

	InputComponent->BindAction<FCameraMovementDelegate>("CameraUp", IE_Pressed, this, &APrimaryPlayerController::HandleCameraVerticalInput, 1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraDown", IE_Pressed, this, &APrimaryPlayerController::HandleCameraVerticalInput, -1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraLeft", IE_Pressed, this, &APrimaryPlayerController::HandleCameraHorizontalInput, -1);
	InputComponent->BindAction<FCameraMovementDelegate>("CameraRight", IE_Pressed, this, &APrimaryPlayerController::HandleCameraHorizontalInput, 1);

	InputComponent->BindAction("CameraUp", IE_Released, this, &APrimaryPlayerController::HandleCameraVerticalCancelled);
	InputComponent->BindAction("CameraDown", IE_Released, this, &APrimaryPlayerController::HandleCameraVerticalCancelled);
	InputComponent->BindAction("CameraLeft", IE_Released, this, &APrimaryPlayerController::HandleCameraHorizontalCancelled);
	InputComponent->BindAction("CameraRight", IE_Released, this, &APrimaryPlayerController::HandleCameraHorizontalCancelled);

	InputComponent->BindAction("UndoMove", IE_Released, this, &APrimaryPlayerController::HandleUndoMovementInput);

	InputComponent->BindAction("Confirm", IE_Released, this, &APrimaryPlayerController::HandleConfirmInput);

}

void APrimaryPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto hud = GetHUD<AGameplayHUD>();

	hud->AssignPlayer(0);
	hud->bDisplayOnBeginPlay = true;
}

AInfectionStrategyPlayerController* APrimaryPlayerController::GetCurrentPlayerController() const
{
	AInfectionStrategyPlayerController* pc = nullptr;

	if (AInfectionStrategyGameMode* mode = Cast<AInfectionStrategyGameMode>(GetWorld()->GetAuthGameMode()))
	{
		pc = Cast<AInfectionStrategyPlayerController>(UGameplayStatics::GetPlayerController(this, mode->GetActivePlayerId()));
	}	

	check(pc);

	return pc;
}

void APrimaryPlayerController::HandleTileMovementInput(const ENeighbor direction)
{
	GetCurrentPlayerController()->TryTileMovement(direction);
}

void APrimaryPlayerController::HandleSelectionInput()
{
	GetCurrentPlayerController()->OnSelectUnitReleased();
}

void APrimaryPlayerController::HandleConfirmInput()
{
	GetCurrentPlayerController()->OnConfirmMoveReleased();
}

void APrimaryPlayerController::HandleCameraVerticalInput(const int32 direction)
{
	GetCurrentPlayerController()->OnMoveCameraVerticalPressed(direction);
}

void APrimaryPlayerController::HandleCameraHorizontalInput(const int32 direction)
{
	GetCurrentPlayerController()->OnMoveCameraHorizontalPressed(direction);
}

void APrimaryPlayerController::HandleCameraVerticalCancelled()
{
	GetCurrentPlayerController()->OnMoveCameraVerticalReleased();
}

void APrimaryPlayerController::HandleCameraHorizontalCancelled()
{
	GetCurrentPlayerController()->OnMoveCameraHorizontalReleased();
}

void APrimaryPlayerController::HandleUndoMovementInput()
{
	GetCurrentPlayerController()->UndoMovement();
}