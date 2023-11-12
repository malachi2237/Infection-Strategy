// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * Base class for player HUD widgets in InfectionStrategy. 
 */
UCLASS()
class UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:

	/** Gets called when the player name is available. Child blueprints should implement to display the name. 
	@param name - The display name of the player
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPlayerDisplayName(const FString &name);
};
