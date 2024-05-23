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

void AGameStateBaseCoop::SetZoneInfo(const EZoneType& Zone, const ELevelSide& Side, const FVector Location)
{
	ZoneType = Zone;
	LevelSide = Side;
	RespawnLoc = Location;
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
		if(Player0GlobalState != NewPlayerState)
		{		
			Player0GlobalState = NewPlayerState;		
			if(NewPlayerState == EPlayerGlobalState::Alive)
			{
				AIManager0->UpdateState(EIAState::REVIVE);
			}
			else
			{			
				AIManager0->UpdateState(EIAState::DEAD);
			}
		}
	}
	else if(PlayerIndex == 1)
	{	
		if(Player1GlobalState != NewPlayerState)
		{		
			Player1GlobalState = NewPlayerState;
			if(NewPlayerState == EPlayerGlobalState::Alive)
			{
				AIManager1->UpdateState(EIAState::REVIVE);
			}
			else
			{			
				AIManager1->UpdateState(EIAState::DEAD);
			}
		}
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
	else if (Player0GlobalState == EPlayerGlobalState::Dead && AIManager1->GetIAcount() <= 1 || Player1GlobalState == EPlayerGlobalState::Dead && AIManager0->GetIAcount() <= 1)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameOverMap);
	}
}

