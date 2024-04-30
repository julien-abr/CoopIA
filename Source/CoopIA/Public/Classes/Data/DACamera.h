// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DACamera.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UDACamera : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	UPROPERTY(EditAnywhere)
	FRotator FollowCamRotation = FRotator(320, 0, 0);
};
