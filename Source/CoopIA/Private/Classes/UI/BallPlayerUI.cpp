// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/UI/BallPlayerUI.h"

#include "Classes/Ball.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABallPlayerUI::ABallPlayerUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABallPlayerUI::Init(ABall* Ball, const UTexture2D* Texture)
{
	BallActor = Ball;
	UpdatePlayerCountUI(Texture);
	
}

// Called when the game starts or when spawned
void ABallPlayerUI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallPlayerUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(BallActor)
		SetActorLocation(BallActor->GetActorLocation());

}

