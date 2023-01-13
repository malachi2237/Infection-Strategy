// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMovementComponent.h"
#include "TileActor.h"


UTileMovementComponent::UTileMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MovementQueue.IsEmpty() && !bCurrentlyMoving)
	{
		FVector tileDirection;
		MovementQueue.Dequeue(tileDirection);

		tileDirection = CalculateTileDirection(tileDirection, 1.f);

		MovementDirection = tileDirection;
		MovementDirection.Normalize();

		CompletionTime = CalculateCompletionTime(tileDirection);
		MovementTime = 0.0f;
		startRotation = PawnOwner->GetActorRotation().Quaternion();
		bCurrentlyMoving = true;
	}

	if (MovementTime < CompletionTime)
	{
		MovementTime += DeltaTime;

		//MovementTime = MovementTime > CompletionTime ? CompletionTime : MovementTime;
		PawnOwner->AddActorWorldOffset(MovementDirection * movementSpeed * DeltaTime);
		PawnOwner->SetActorRotation(FQuat::Slerp(startRotation, MovementDirection.ToOrientationQuat(), MovementTime / CompletionTime));
	}
	else
		bCurrentlyMoving = false;
}

FVector UTileMovementComponent::CalculateTileDirection(const FVector& direction, float scale) const
{
	const ATileActor *targetTile = ATileActor::GetTileUnderLocation(PawnOwner->GetActorLocation() + direction * scale);
	const ATileActor *currentTile = ATileActor::GetTileUnderLocation(PawnOwner->GetActorLocation());
	FVector tileDirection = FVector::Zero();

	if (targetTile && currentTile)
	{
		// Ensure that the tiles are neighbors
		for (int i = 0; i < 4/*restricts looking for current tile in cardinal direction of the target*/; i++)
		{
			if (targetTile->neighbors[i] == currentTile)
			{
				tileDirection = targetTile->GetActorLocation() - currentTile->GetActorLocation();
				break;
			}
		}
	}
		
	return tileDirection;
}
