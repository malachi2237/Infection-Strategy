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

		CompletionTime = CalculateCompletionTime(tileDirection);
		MovementTime = 0.0f;

		StartRotation = PawnOwner->GetActorRotation().Quaternion();
		StartLocation = PawnOwner->GetActorLocation();

		EndLocation = ATileActor::GetTileUnderLocation(StartLocation + tileDirection)->GetActorLocation();
		EndRotation = tileDirection.ToOrientationQuat();

		bCurrentlyMoving = true;
	}

	if (MovementTime < CompletionTime)
	{
		MovementTime += DeltaTime;

		MovementTime = MovementTime > CompletionTime ? CompletionTime : MovementTime;

		PawnOwner->SetActorLocation(FMath::Lerp(StartLocation, EndLocation, MovementTime / CompletionTime));
		PawnOwner->SetActorRotation(FQuat::Slerp(StartRotation, EndRotation, MovementTime / CompletionTime));
	}
	else if (bCurrentlyMoving)
	{
		OnMovementComplete.Broadcast();
		bCurrentlyMoving = false;
	}
		
}

FVector UTileMovementComponent::CalculateTileDirection(const FVector& direction, const float scale) const
{
	const ATileActor *targetTile = ATileActor::GetTileUnderLocation(PawnOwner->GetActorLocation() + direction * scale);
	const ATileActor *currentTile = ATileActor::GetTileUnderLocation(PawnOwner->GetActorLocation());
	FVector tileDirection = FVector::Zero();

	if (targetTile && currentTile)
	{
		// Ensure that the tiles are neighbors
		for (int i = 0; i < 4/*restricts looking for current tile in cardinal direction of the target*/; i++)
		{
			if (targetTile->Neighbors[i] == currentTile)
			{
				tileDirection = targetTile->GetActorLocation() - PawnOwner->GetActorLocation();
				break;
			}
		}
	}
		
	return tileDirection;
}
