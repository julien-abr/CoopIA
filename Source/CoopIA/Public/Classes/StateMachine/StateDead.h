// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/StateMachine/State.h"
#include "StateDead.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UStateDead : public UState
{
	GENERATED_BODY()
public:
	virtual void OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef) override;
	virtual void OnStateTick() override;
	virtual void OnStateLeave() override;
};
