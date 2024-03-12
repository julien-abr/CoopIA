// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Laser.h"

#include "Components/ArrowComponent.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	_body->SetupAttachment(RootComponent);
	_body->SetWorldScale3D(FVector(0.5f, 0.1f, 0.1f));

	_firePoint = CreateDefaultSubobject<UArrowComponent>("FirePoint");
	_firePoint->SetupAttachment(_body);
	_firePoint->SetRelativeLocation(FVector(0.25f, 0, 0.05f));
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

