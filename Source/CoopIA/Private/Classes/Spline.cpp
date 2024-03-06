// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Spline.h"

#include "Components/SplineComponent.h"

// Sets default values
ASpline::ASpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

