// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerGlobalState.generated.h"

UENUM(BlueprintType)
enum class EPlayerGlobalState : uint8
{
	Alive  UMETA(DisplayName= "Alive"),
	Dead   UMETA(DisplayName= "Dead")
};