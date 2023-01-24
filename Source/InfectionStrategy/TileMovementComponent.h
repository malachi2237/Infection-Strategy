// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TileMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INFECTIONSTRATEGY_API UTileMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UTileMovementComponent();
	
protected:
	UPROPERTY(EditAnywhere)
	float movementSpeed = 500.0f;

	bool bCurrentlyMoving = false;
private:
	float MovementTime = 0.0f;
	float CompletionTime = 0.0f;

	FVector StartLocation;
	FQuat StartRotation;
	
	FVector EndLocation;
	FQuat EndRotation;

	TQueue<FVector> MovementQueue;

	FORCEINLINE float CalculateCompletionTime(const FVector &toLocation) const { return toLocation.Length() / movementSpeed; };
	FVector CalculateTileDirection(const FVector &direction, float scale) const;
	
public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override { MovementQueue.Enqueue(WorldVector); };
	
	FORCEINLINE bool IsMoving() const { return !MovementQueue.IsEmpty() || bCurrentlyMoving; };
};
