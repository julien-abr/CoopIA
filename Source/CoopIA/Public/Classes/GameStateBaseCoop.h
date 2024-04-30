// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Classes/Data/Enum/ZoneType.h"
#include "Classes/Data/Enum/PlayerGlobalState.h"
#include "GameStateBaseCoop.generated.h"

class ADeathManager;
/**
 * 
 */
UCLASS()
class COOPIA_API AGameStateBaseCoop : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateBaseCoop();
	
	EZoneType GetZoneType() {return ZoneType;}

	EPlayerGlobalState GetPlayer0GlobalState() {return Player0GlobalState;}
	EPlayerGlobalState GetPlayer1GlobalState() {return Player1GlobalState;}
	
private:
	EZoneType ZoneType;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	EPlayerGlobalState Player0GlobalState;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	EPlayerGlobalState Player1GlobalState;

	UPROPERTY(EditAnywhere)
	TAssetPtr<UWorld> GameOverMap;
	
	bool bGameIsPaused = false;

	UPROPERTY()
	TObjectPtr<ADeathManager> DeathManager;

	UFUNCTION()
	void OnPlayerGlobalStateChanged(int32 PlayerIndex, EPlayerGlobalState NewPlayerState);

	void CheckGameOver();
};
