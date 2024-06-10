
// Fill out your copyright notice in the Description page of Project Settings.

#include "Classes/Ball.h"

#include "EnhancedInputComponent.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Classes/AIManager.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Data/EIAState.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

	if(!ST)
	{
		if(APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(NewController))
		{
			ST = PlayerControllerBase->GetStateMachineComponent();
		}
	}
	
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

EIAState ABall::GetAIState_Implementation()
{
	return EIAState::BALL;
}

int32 ABall::GetPlayerIndex_Implementation()
{
	if (ST)
		return ST->GetPlayerIndex();
	return 0;
}

bool ABall::CheckIsFalling()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - (FVector::UpVector * 150);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params, FCollisionResponseParams());

	if (HitResult.GetActor())
		return false;

	return true;
}

void ABall::StartSpear()
{
	if (!CheckIsFalling())
		ST->UpdateState(SpearTag);
}

void ABall::StartNeutral()
{
	if (!CheckIsFalling())
		ST->UpdateState(NeutralTag);
}

void ABall::StartShield()
{
	if (!CheckIsFalling())
		ST->UpdateState(ShieldTag);
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