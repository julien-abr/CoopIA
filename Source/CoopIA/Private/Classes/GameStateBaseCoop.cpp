// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/GameStateBaseCoop.h"
#include "Classes/AIManager.h"
#include "Classes/DeathManager.h"

//Libraries
#include "Classes/Data/EIAState.h"
#include "Classes/AIManager.h"
#include "Kismet/GameplayStatics.h"

AGameStateBaseCoop::AGameStateBaseCoop()
{
	//Get DeathManager and Subscribe to the Delegate
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADeathManager::StaticClass());
	DeathManager = Cast<ADeathManager>(Actor);

	if(DeathManager)
	{
		DeathManager->OnPlayerGlobalStateChangedDelegate.AddUObject(this, &AGameStateBaseCoop::OnPlayerGlobalStateChanged);
	}
}

void AGameStateBaseCoop::Init(TArray<AAIManager*>& ArrayAIManager)
{
	AIManager0 = ArrayAIManager[0];
	AIManager1 = ArrayAIManager[1];
}

void AGameStateBaseCoop::SetZoneInfo(const EZoneType& Zone, const ELevelSide& Side)
{
	ZoneType = Zone;
	LevelSide = Side;
}

const AActor* AGameStateBaseCoop::GetPlayer(int Index)
{
	if(Index == 0)
	{	
		return AIManager0->GetCurrentActor();
	}
	else if(Index == 1)
	{
		return AIManager1->GetCurrentActor();
	}
	return nullptr;
}

void AGameStateBaseCoop::OnPlayerGlobalStateChanged(int32 PlayerIndex, EPlayerGlobalState NewPlayerState)
{
	if(PlayerIndex == 0)
	{
		Player0GlobalState = NewPlayerState;
		AIManager0->UpdateState(EIAState::DEAD);
	}
	else if(PlayerIndex == 1)
	{
		Player1GlobalState = NewPlayerState;
		AIManager1->UpdateState(EIAState::DEAD);
	}

	CheckGameOver();
}

void AGameStateBaseCoop::CheckGameOver()
{
	if(Player0GlobalState == EPlayerGlobalState::Dead && Player1GlobalState == EPlayerGlobalState::Dead)
	{
		//Play game over menu
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameOverMap);
	}
}
