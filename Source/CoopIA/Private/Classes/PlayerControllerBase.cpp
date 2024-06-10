// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/PlayerControllerBase.h"

#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/StateMachine/StateMachineComponent.h"

APlayerControllerBase::APlayerControllerBase()
{
	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>(TEXT("StateMachineComponent"));
	AddOwnedComponent(StateMachineComponent);
}

void APlayerControllerBase::Init(AMainCamera* Camera, const int Index)
{
	MainCamera = Camera;
	PlayerIndex = Index;
}

UStateMachineComponent* APlayerControllerBase::GetStateMachineComponent() const
{
	return StateMachineComponent;
}

const int APlayerControllerBase::GetPlayerIndex() const
{
	return PlayerIndex;
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetViewTargetWithBlend(MainCamera);

	if(StateMachineComponent && !bDoOncePosses)
	{
		bDoOncePosses = true;
		UE_LOG(LogTemp, Warning, TEXT("Call Init from PC"));
		StateMachineComponent->Init(this, DA_StateMachine);
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn());
		if(CharacterBase)
		{
			CharacterBase->Init(StateMachineComponent);
		}
	}
}
