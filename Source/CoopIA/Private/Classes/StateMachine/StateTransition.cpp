// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateTransition.h"

#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Classes/StateMachine/StateMachineComponent.h"

void UStateTransition::OnStateEnter(UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	UE_LOG(LogTemp, Warning, TEXT("Enter STATE TRANSITION"));

	 
	const bool bShouldPlayTransition = (ST->NextTag != FGameplayTag::EmptyTag && ST->NextTag != ST->DA_StateMachine->NeutralState);

	if(bShouldPlayTransition)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			//VFX HERE
			ST->UpdateStateFromTransition(ST->NextTag);
		}), 0.5, false);
	}
	else
	{
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
}
