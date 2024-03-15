// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Shield.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShield::Hide()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void AShield::Show()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}


