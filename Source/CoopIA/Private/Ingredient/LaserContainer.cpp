// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/LaserContainer.h"

// Sets default values
ALaserContainer::ALaserContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaserContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserContainer::RayHitAction_Implementation()
{
	if (!_door)
		return;

	AActor* doorActor = Cast<AActor>(_door);
	if (doorActor)
		doorActor->Destroy();
}