// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Templates/SubclassOf.h"
#include "ITurnBased.h"
#include "GameplayHUD.generated.h"

class UUserWidget;
class UVehicleWidget;
class AVehicleUnit;

/**
 * 
 */
UCLASS()
class AGameplayHUD : public AHUD, public ITurnBased
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void AssignPlayer(const int32 player) { OwningPlayerId = player; };

	UFUNCTION()
	void SelectVehicle(const AVehicleUnit* const vehicle);

	UFUNCTION()
	void DeselectVehicle();

	/** TurnBased Interface */
	virtual void OnTurnBegin(const int32 player) override;
	virtual void OnTurnEnd(const int32 player) override;
	/** End TurnBased Interface */

	/** Instance of the HudTemplate */
	UPROPERTY()
		UUserWidget* HudInstance;

	/** Instance of the VehicleHudTemplate */
	UPROPERTY()
		UVehicleWidget* VehicleHudInstance;

	bool bDisplayOnBeginPlay = false;

private:

	/** Template for the HUD widget */
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget> HudTemplate;

	/** Template for the HUD displayed when a vehicle is selected */
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UVehicleWidget> VehicleHudTemplate;

	int32 OwningPlayerId = -1;
};
