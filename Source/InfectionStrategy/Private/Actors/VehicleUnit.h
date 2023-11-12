 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITargetable.h"
#include "ITurnBased.h"
#include "VehicleUnit.generated.h"

class ATileActor;
class AUnitState;

class USelectionStateComponent;
class UTileMovementComponent;

/** Base class for vehicles to be used as selectable units */
UCLASS()
class INFECTIONSTRATEGY_API AVehicleUnit : public APawn, public ITargetable, public ITurnBased
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicleUnit();
	
	/* Pawn Interface */
	virtual bool ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	/* End Pawn interface */

	/** Attempts to select the vehicle for action.
	 * @param playerId - Id of the player attempting selection
	 * @return True on success, false on failure
	 */
	bool TrySelect(int32 playerId);

	/** Deselects the vehicle for action. */
	void Deselect();

	/** Checks if the unit is able to move again this turn for preview purposes.
	 * @return True if the vehicle can move, false if not
	 * @warning This method does not actually perform movement but internally prepares it.
	 */
	bool TryMovement();

	/** Tells the unit to ignore a number of previous movements this turn.
	 * @param steps - Amount of movements to ignore
	 */
	void UndoMovement(int32 steps);

	/** Gets the remaining amount of movement the vehicle can perform this turn. */
	int32 RemainingMoves() const;

	bool CanAttack() const;

	/** Returns the health of the unit when it was spawned. */
	int32 GetMaxHealth() const { return Health; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	/* Targetable interface */
	void Target() override;
	void Untarget() override;
	/* End Targetable interface */

	/* TurnBased interface*/
	virtual void OnTurnBegin(const int32 playerId) override;
	virtual void OnTurnEnd(const int32 playerId) override;
	virtual void OnMatchEnd(const int32 winnerId) override {};
	/* End TurnBased interface */

	/** The tile on which the vehicle is currently located */
	UPROPERTY()
	ATileActor* Tile;

	UPROPERTY()
	AUnitState* UnitState;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	/** Remaining health of the vehicle */
	UPROPERTY(EditAnywhere)
	int32 Health;

	// attack statistics

	/** The amount of damage the vehicle deals to another vehicle on direct hit */
	UPROPERTY(EditAnywhere)
	int32 DirectPhysicalDamage;

	/** The amount of damage the vehicle deals to a tile's gas level on direct hit */
	UPROPERTY(EditAnywhere)
	int32 DirectGasDamage;

	/** The distance in tiles from which the vehicle can target an actor for direct damage */
	UPROPERTY(EditAnywhere)
	int32 AttackRange;

	//movement statistics and other state variables

	/** Maximum amount of movement per turn, in tiles */
	UPROPERTY(EditAnywhere)
	int32 MaxMovement = 10;

	//** Number of tiles moved this turn */
	int32 CurrentMovement = 0;

	/** Is the vehicle able to attack this turn? */
	bool bCanAttack = true;

	//Components

	/** Mesh representing the vehicle */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	/** Component controlling the different selection states the vehicle can be in */
	UPROPERTY(VisibleAnywhere)
	USelectionStateComponent* SelectionState;

	//Selection Properties

	/** Material used by the vehicle when selected for action */
	UPROPERTY(EditAnywhere)
	UMaterialInstance *SelectedStateMaterial;

	/** Movement component allowing vehicle to move according to the tile grid */
	UPROPERTY(EditAnywhere)
	UTileMovementComponent* TileMovement;

	/** Callback for when the movement component completes movement */
	UFUNCTION()
	void OnMovementComplete();

	/** Callback for when the vehicle is selected for action */
	void OnSelection();

	/** Checks if the vehicle can still move this turn */
	bool CanMove() const;
	

private:

};
