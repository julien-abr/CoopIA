// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/State.h"
#include "Classes/StateMachine/StateMachineComponent.h"

void UState::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	ST = StateMachineComponentRef;
}

void UState::OnStateTick()
{
}

void UState::OnStateLeave()
{
}
