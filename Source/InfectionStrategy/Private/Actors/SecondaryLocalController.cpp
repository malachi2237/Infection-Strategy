// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SecondaryLocalController.h"
#include "Widgets/GameplayHUD.h"

void ASecondaryLocalController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto hud = GetHUD<AGameplayHUD>();

	hud->AssignPlayer(1);
}