// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_IA.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UDA_IA : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
    float RandomMoveTime = 2.f;

	UPROPERTY(EditAnywhere)
	float RandomMoveDistanceFromPlayer = 200.f;

	UPROPERTY(EditAnywhere)
	float TransfoDistanceFromPlayer = 200.f;

	UPROPERTY(EditAnywhere)
	float BaseAcceptanceRadius = 10.f;

	UPROPERTY(EditAnywhere)
	float RetryAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere)
	float CollisionImpulseMultiplier = 2.f;
};
