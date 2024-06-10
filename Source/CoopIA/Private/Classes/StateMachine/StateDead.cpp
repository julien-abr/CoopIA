// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateDead.h"

#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/StateMachine/StateMachineComponent.h"

void UStateDead::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	
	UE_LOG(LogTemp, Warning, TEXT("Enter Dead"));
	ST->DestroyIA();
	ST->MainCamera->SetPlayer(nullptr, ST->PlayerIndex);
	
	ST->HidePrevious();
	ST->PlayerController->UnPossess();
	ST->PlayerController->SetControlRotation(FRotator());

	ST->Player->SetActorLocation(ST->PlayerLastHexPos, false, nullptr, ETeleportType::TeleportPhysics);
	ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	ST->PlayerController->Possess(ST->Player);
	ST->Player->Died();
	ST->CurrentActor = ST->Player;
	ST->MainCamera->SetPlayer(ST->Player, ST->PlayerIndex);
}

void UStateDead::OnStateTick()
{
	Super::OnStateTick();
}

void UStateDead::OnStateLeave()
{
	Super::OnStateLeave();
}