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
	
	UFUNCTION(BlueprintImplementableEvent)
    void UpdateUI(const int playerIndex, const UTexture2D* texture);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIACount(const int playerIndex, const int Count);


private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TSubclassOf<ACharacterBase> PlayerToSpawn;

	int16 arrayIndex = 0;
};



