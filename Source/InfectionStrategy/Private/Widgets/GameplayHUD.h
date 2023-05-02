// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Templates/SubclassOf.h"
#include "ITurnBased.h"
#include "GameplayHUD.generated.h"

class UUserWidget;
class UVehicleWidget;
class UPlayerHUDWidget;
class AVehicleUnit;

/**
 * A class which controls the HUD for InfectionStrategy. 
 */
UCLASS()
class AGameplayHUD : public AHUD, public ITurnBased
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void BeginPlay() override;

	/** Assigns the player whose turn this HUD will appear. 
	 * @param player - The player to assign this HUD.
	 */
	UFUNCTION()
	void AssignPlayer(const int32 player);

	/** Updates the HUD to display UI to control a vehicle.
	 * @param vehicle - The vehicle to be controlled
	 */
	UFUNCTION()
	void SelectVehicle(const AVehicleUnit* const vehicle);

	/** Removes vehicle UI from the screen and forgets the last unit selected. */
	UFUNCTION()
	void DeselectVehicle();

	/** TurnBased Interface */
	virtual void OnTurnBegin(const int32 player) override;
	virtual void OnTurnEnd(const int32 player) override;
	/** End TurnBased Interface */

	/** Instance of the HudTemplate */
	UPROPERTY()
	UPlayerHUDWidget* HudInstance;

	/** Instance of the VehicleHudTemplate */
	UPROPERTY()
	UVehicleWidget* VehicleHudInstance;

	/** Should the HUD display on BeginPlay? */
	bool bDisplayOnBeginPlay = false;

private:

	/** Template for the HUD widget */
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> HudTemplate;

	/** Template for the HUD displayed when a vehicle is selected */
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UVehicleWidget> VehicleHudTemplate;

	/** The player that is assigned to this HUD */
	int32 OwningPlayerId = -1;
};
