// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Classes/Data/Enum/ZoneType.h"
#include "Classes/Data/Enum/PlayerGlobalState.h"
#include "Data/Enum/ELevelSide.h"
#include "GameStateBaseCoop.generated.h"

class ADeathManager;
class AAIManager;
/**
 * 
 */
UCLASS()
class COOPIA_API AGameStateBaseCoop : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateBaseCoop();
	
	void Init(TArray<AAIManager*>& ArrayAIManager);
	
	EZoneType GetZoneType() { return ZoneType; }
	ELevelSide GetLevelSide() {return LevelSide;}
	const FVector GetRespawnLoc() { return RespawnLoc; }

	void SetZoneInfo(const EZoneType& Zone, const ELevelSide& Side, const FVector Location);

	const AActor* GetPlayer(int Index);

	EPlayerGlobalState GetPlayer0GlobalState() {return Player0GlobalState;}
	EPlayerGlobalState GetPlayer1GlobalState() {return Player1GlobalState;}
	
private:
	UPROPERTY()
	TObjectPtr<AAIManager> AIManager0;

	UPROPERTY()
	TObjectPtr<AAIManager> AIManager1;
	
	EZoneType ZoneType = EZoneType::Running;

	FVector RespawnLoc;

	ELevelSide LevelSide = ELevelSide::MIDDLE;

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
