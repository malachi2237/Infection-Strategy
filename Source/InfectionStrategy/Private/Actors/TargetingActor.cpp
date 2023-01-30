// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingActor.h"

#include "Components/ShapeComponent.h"

// Sets default values
ATargetingActor::ATargetingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Targeting Shape"));
	SetRootComponent(collisionMesh);

	collisionMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	
}

// Called when the game starts or when spawned
void ATargetingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetingActor::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlapBeginDelegate.ExecuteIfBound(OtherActor);
}

void ATargetingActor::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnOverlapEndDelegate.ExecuteIfBound(OtherActor);
}
