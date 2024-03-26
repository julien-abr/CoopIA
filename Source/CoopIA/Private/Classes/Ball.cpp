// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Ball.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Classes/AIManager.h"
#include "Classes/Data/EIAState.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABall::SetAIManager(class AAIManager* Manager)
{
	AIManager = Manager;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABall::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(FormationMappingContext, 0);
			Subsystem->AddMappingContext(BallMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Ball Move
		//EnhancedInputComponent->BindAction(BallMoveAction, ETriggerEvent::Started, this, &ABall::StartSpear);)
		
		//Spear
		EnhancedInputComponent->BindAction(SpearAction, ETriggerEvent::Started, this, &ABall::StartSpear);

		//Neutral
		EnhancedInputComponent->BindAction(NeutralAction, ETriggerEvent::Started, this, &ABall::StartNeutral);
		
		//Shield
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Started, this, &ABall::StartShield);
	}
}

void ABall::StartSpear()
{
	AIManager->UpdateState(EIAState::SPEAR);
}

void ABall::StartNeutral()
{
	AIManager->UpdateState(EIAState::RANDOM_MOVE);
}

void ABall::StartShield()
{
	AIManager->UpdateState(EIAState::SHIELD);
}

void ABall::Hide()
{
	SetSimulatePhysics(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ABall::Show()
{
	SetSimulatePhysics(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}