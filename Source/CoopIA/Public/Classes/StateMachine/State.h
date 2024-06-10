// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "State.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COOPIA_API UState : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UStateMachineComponent> ST;
public:
	virtual void OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef);
	virtual void OnStateTick();
	virtual void OnStateLeave();

	
	
};
