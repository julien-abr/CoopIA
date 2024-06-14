// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/UI/TransitionEffectPlayer.h"

// Sets default values
ATransitionEffectPlayer::ATransitionEffectPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransitionEffectPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransitionEffectPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

