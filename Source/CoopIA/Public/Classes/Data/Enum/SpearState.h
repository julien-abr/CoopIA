// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SpearState.generated.h"

UENUM(BlueprintType)
enum class ESpearState 
{
	STATIC = 0    UMETA(DisplayName = "RANDOM_MOVE"),
	DASHING = 1       UMETA(DisplayName = "MOVE_TO")
};

