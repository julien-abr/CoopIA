// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameStateBase.h"
#include "Classes/Data/Enum/ZoneType.h"
#include "Classes/Data/Enum/PlayerGlobalState.h"
#include "Data/Enum/ELevelSide.h"
#include "GameStateBaseCoop.generated.h"

class UStateMachineComponent;
class ADeathManager;
class AAIManager;
class APlayerControllerBase;
/**
 * 
 */
UCLASS()
class COOPIA_API AGameStateBaseCoop : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateBaseCoop();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();
	
	void Init(TArray<APlayerControllerBase*>& ArrayPlayerController);
	
	EZoneType GetZoneType() const { return ZoneType; }
	ELevelSide GetLevelSide() const {return LevelSide;}
	FVector GetRespawnLoc() const { return RespawnLoc; }

	void SetZoneInfo(const EZoneType& Zone, const ELevelSide& Side);

	void SetZoneRespawnLocation(const FVector& Location);

	const AActor* GetPlayer(int Index) const;

	EPlayerGlobalState GetPlayer0GlobalState() const {return Player0GlobalState;}
	EPlayerGlobalState GetPlayer1GlobalState() const {return Player1GlobalState;}
	
private:
	UPROPERTY()
	TObjectPtr<UStateMachineComponent> ST_Player0;

	UPROPERTY()
	TObjectPtr<UStateMachineComponent> ST_Player1;
	
	UPROPERTY(VisibleAnywhere)
	EZoneType ZoneType = EZoneType::Running;

	UPROPERTY(VisibleAnywhere)
	FVector RespawnLoc;

	UPROPERTY(EditAnywhere)
	FGameplayTag DeadTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag ReviveTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameOverTag;

	UPROPERTY(VisibleAnywhere)
	ELevelSide LevelSide = ELevelSide::MIDDLE;

	UPROPERTY(VisibleAnywhere)
	EPlayerGlobalState Player0GlobalState;
	
	UPROPERTY(VisibleAnywhere)
	EPlayerGlobalState Player1GlobalState;

	UPROPERTY(EditAnywhere)
	TAssetPtr<UWorld> GameOverMap;
	
	bool bGameIsPaused = false;

	UPROPERTY()
	TObjectPtr<ADeathManager> DeathManager;

	UFUNCTION()
	void OnPlayerGlobalStateChanged(int32 PlayerIndex, EPlayerGlobalState NewPlayerState);

	bool IsGameOver();
};
