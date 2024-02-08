// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Classes/CharacterBase.h"
#include "GameFramework/GameModeBase.h"
#include "CoopIAGameMode.generated.h"

UCLASS(minimalapi)
class ACoopIAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoopIAGameMode();

	virtual void BeginPlay() override;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TSubclassOf<ACharacterBase> PlayerToSpawn;
};



