// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneType.generated.h"

UENUM(BlueprintType)
enum class EZoneType : uint8
{
	Puzzle  UMETA(DisplayName= "Puzzle"),
	Running   UMETA(DisplayName= "Running")
};
