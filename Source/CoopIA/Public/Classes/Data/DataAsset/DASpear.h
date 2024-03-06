// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DASpear.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UDASpear : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float SpearHoldTimer = 2.f;

	UPROPERTY(EditAnywhere)
	float TimerDash = 3.f;

	UPROPERTY(EditAnywhere)
	float TimerBetweenDashUPForward = .1f;

	UPROPERTY(EditAnywhere)
	float DashZBaseVelocity = 200.f;
	
	UPROPERTY(EditAnywhere)
	float SpearMinPower = 0.f;
	
	UPROPERTY(EditAnywhere)
	float SpearMaxPower = 1000.f;
};
