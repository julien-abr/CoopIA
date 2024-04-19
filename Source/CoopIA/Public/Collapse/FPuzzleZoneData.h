// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Enum/ELevelSide.h"

#include "FPuzzleZoneData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct COOPIA_API FPuzzleZoneData
{
	GENERATED_BODY()

public:
	FPuzzleZoneData();
	~FPuzzleZoneData();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool watchOnMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ELevelSide side;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int startZone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int endZone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float duration;
};
