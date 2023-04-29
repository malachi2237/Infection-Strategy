// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayHUD.h"
#include "Blueprint/UserWidget.h"
#include "VehicleWidget.h"
#include "PlayerHUDWidget.h"

void AGameplayHUD::BeginPlay()
{
	APlayerController* controller = GetOwningPlayerController();

	if (controller && controller->GetPawn())
	{
		/* This block include additional checking to see if BeginPlay has alreaedy been called. 
			This is because there has been a problem with it being called multiple times. Most likely,
			this is due to calling "DispatchBeginPlay" in the Controller */

		if (HudTemplate && !HudInstance)
			HudInstance = CreateWidget<UPlayerHUDWidget>(controller, HudTemplate);

		check(HudInstance);

		if (VehicleHudTemplate && !VehicleHudInstance)
			VehicleHudInstance = CreateWidget<UVehicleWidget>(controller, VehicleHudTemplate);

		check(VehicleHudInstance);

		if (bDisplayOnBeginPlay)
		{
			HudInstance->AddToViewport();
			bDisplayOnBeginPlay = false;
		}
	}
}

void AGameplayHUD::AssignPlayer(const int32 player)
{
	OwningPlayerId = player;

	check(HudInstance);
	HudInstance->SetPlayerDisplayName(FString::FromInt(player + 1));
};

void AGameplayHUD::SelectVehicle(const AVehicleUnit* const vehicle)
{
	if (!VehicleHudInstance->IsInViewport())
		VehicleHudInstance->AddToViewport();

	VehicleHudInstance->SetSelectedVehicle(vehicle);
}

void AGameplayHUD::DeselectVehicle()
{
	VehicleHudInstance->RemoveFromViewport();
	VehicleHudInstance->SetSelectedVehicle(nullptr);
}

void AGameplayHUD::OnTurnBegin(const int32 player)
{
	if (player == OwningPlayerId)
		HudInstance->AddToViewport();
}

void AGameplayHUD::OnTurnEnd(const int32 player)
{
	HudInstance->RemoveFromViewport();

	DeselectVehicle();
}