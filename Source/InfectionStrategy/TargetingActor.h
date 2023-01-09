// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Actor.h"
#include "TargetingActor.generated.h"

DECLARE_DELEGATE_OneParam(FOnOverlapDelegate, AActor*)

UCLASS(abstract)
class INFECTIONSTRATEGY_API ATargetingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetingActor();

	FOnOverlapDelegate OnOverlapBeginDelegate;
	FOnOverlapDelegate OnOverlapEndDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* collisionMesh;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//AActor* owner = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void SetOwner(AActor* newOwner);
};
