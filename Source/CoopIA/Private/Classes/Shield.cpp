// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Shield.h"

#include "EnhancedInputSubsystems.h"
#include "Classes/CharacterBase.h"

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

void AShield::SetOwner(class ACharacterBase* Character)
{
	m_Player = Character;
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_Player)
	{
		UpdateShieldLocation();
	}
}

void AShield::UpdateShieldLocation()
{
	SetActorLocation(m_Player->GetActorLocation());
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


