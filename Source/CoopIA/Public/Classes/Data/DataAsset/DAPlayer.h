// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UDAPlayer : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 500.f;
};
