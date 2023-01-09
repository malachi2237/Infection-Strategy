 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionStateComponent.h"
#include "TileActor.h"
#include "ITargetable.h"
#include "TileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "VehicleUnit.generated.h"

UCLASS()
class INFECTIONSTRATEGY_API AVehicleUnit : public APawn, public ITargetable
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
	int32 RemainingMoves();

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

	virtual FORCEINLINE UPawnMovementComponent* GetMovementComponent() const override { return tileMovement; };

	void SetPlayerOwner(int32 newOwner);
	void Target() override;
	void Untarget() override;
};
