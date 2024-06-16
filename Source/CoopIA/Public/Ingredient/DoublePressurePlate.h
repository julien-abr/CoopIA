// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Ingredient/PressurePlate.h"
#include "DoublePressurePlate.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API ADoublePressurePlate : public APressurePlate
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* triggerBox;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* secondBody;

	ADoublePressurePlate();
};
