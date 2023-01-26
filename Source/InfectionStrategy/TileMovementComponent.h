// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TileMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementCompletedDelegate);

/** A movement component for any pawn moving on the TileSystem. The class builds a queue of movements to be executed and locks them to the tile grid. */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INFECTIONSTRATEGY_API UTileMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UTileMovementComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override { MovementQueue.Enqueue(WorldVector); };

	/** Checks if the component is currently moving the pawn or preparing to move it. */
	FORCEINLINE bool IsMoving() const { return !MovementQueue.IsEmpty() || bCurrentlyMoving; };

	/** Delegate that is executed when the movement is complete */
	UPROPERTY(BlueprintAssignable)
	FOnMovementCompletedDelegate OnMovementComplete;

protected:
	/** Unit per second movement speed */
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 0.0f;

	/** Is the component in the process of moving the pawn? */
	bool bCurrentlyMoving = false;

private:

	/** Time elapsed while performing the current movement */
	float MovementTime = 0.0f;

	/** The amount of time it will take to complete the current movement */
	float CompletionTime = 0.0f;

	/** Start location of the current movement */
	FVector StartLocation;

	/** Start rotation of the current movement */
	FQuat StartRotation;
	
	/** Target location of the current movement */
	FVector EndLocation;

	/** Target rotation of the current movement */
	FQuat EndRotation;

	/** Queue of movements to be performed right now */
	TQueue<FVector> MovementQueue;

	/** Determines how long it will take to complete a movement.
	 * @param toLocation - The target location 
	 * @return - Time to complete the movement in seconds
	 */
	FORCEINLINE float CalculateCompletionTime(const FVector &toLocation) const { return toLocation.Length() / MovementSpeed; };

	/** Determines the actual direction from the current tile to the target one, if the tiles are neighbors
	 * @param direction - the direction to look for the target tile
	 * @param scale - scalar multiplier for direction
	 * @return The direction of the next tile from the current one. Zero Vector if the tiles are not neighbors */
	FVector CalculateTileDirection(const FVector &direction, const float scale) const;
};
