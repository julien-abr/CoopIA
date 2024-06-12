// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateTransition.h"

#include "Classes/CharacterBase.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UStateTransition::OnStateEnter(UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	UE_LOG(LogTemp, Warning, TEXT("Enter STATE TRANSITION"));

	if(ST->Player)
		ST->Player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	
	if(ST->NextTag != FGameplayTag::EmptyTag && ST->NextTag != ST->DA_StateMachine->NeutralState)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			//VFX HERE
			UE_LOG(LogTemp, Warning, TEXT("Play vfx transition"));
			ST->UpdateStateFromTransition(ST->NextTag);
		}), ST->DA_StateMachine->TransitonTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No vfx transition"));
		ST->UpdateStateFromTransition(ST->NextTag);
	}
}

void UStateTransition::OnStateTick()
{
	Super::OnStateTick();
}

void UStateTransition::OnStateLeave()
{
	Super::OnStateLeave();
	UE_LOG(LogTemp, Warning, TEXT("TRANSITION => OK"));
	if (ST->OnHidePrevious.IsBound())
	{
		ST->OnHidePrevious.Execute();
	}
}
