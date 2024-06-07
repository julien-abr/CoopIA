// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COOPIA_API IInteract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Wind(FVector windDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateRail(FVector railDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeactivateRail();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LockPropulseBoule(bool locking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LaunchPropulseBoule(FVector launchDirection, float canonForce, float maxVelocity);
};
