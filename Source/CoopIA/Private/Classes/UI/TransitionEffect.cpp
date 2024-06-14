// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/UI/TransitionEffect.h"

// Sets default values
ATransitionEffect::ATransitionEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransitionEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransitionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

