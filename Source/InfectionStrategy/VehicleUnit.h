 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITargetable.h"
#include "ITurnBased.h"
#include "VehicleUnit.generated.h"

class ATileActor;
class USelectionStateComponent;
class UTileMovementComponent;

UCLASS()
class INFECTIONSTRATEGY_API AVehicleUnit : public APawn, public ITargetable, public ITurnBased
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicleUnit();
	
	ATileActor* tile;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	int32 owner = 0;

	UPROPERTY(EditAnywhere)
	int32 health;

	// attack statistics
	UPROPERTY(EditAnywhere)
	int32 directPhysicalDamage;

	UPROPERTY(EditAnywhere)
	int32 directGasDamage;

	UPROPERTY(EditAnywhere)
	int32 attackRange;

	//movement statistics and other state variables
	UPROPERTY(EditAnywhere)
	int32 maxMovement = 10;

	UPROPERTY(EditAnywhere)
	int32 currentMovement = 0;

	bool bCanAttack = true;

	//Components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
	USelectionStateComponent* selectionState;

	//Selection Properties
	UPROPERTY(EditAnywhere)
	UMaterialInstance *selectedStateMaterial;

	UTileMovementComponent* tileMovement;

	void OnSelection();

	bool CanMove();

private:
	UPROPERTY(VisibleAnywhere)
	float movementSpeed = 100.0f;

	float currentMovementTimer = 0.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool TrySelect(int32 playerId);
	void Deselect();

	bool TryMovement();
	void UndoMovement(int32 steps);

	int32 RemainingMoves();

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void SetPlayerOwner(int32 newOwner);

	/* Targetable interface */
	void Target() override;
	void Untarget() override;

	/* TurnBased Interface*/
	virtual void OnTurnBegin(int32 playerId) override;
	virtual void OnTurnEnd(int32 playerId) override;
};
