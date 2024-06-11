// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/GameStateBaseCoop.h"
#include "Classes/AIManager.h"
#include "Classes/DeathManager.h"
#include "Classes/PlayerControllerBase.h"

//Libraries
#include "Classes/StateMachine/StateMachineComponent.h"
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

void AGameStateBaseCoop::Init(TArray<APlayerControllerBase*>& ArrayPlayerController)
{
	for (const auto PlayerController : ArrayPlayerController)
	{
		if(PlayerController->GetPlayerIndex() == 0)
		{
			ST_Player0 = PlayerController->GetStateMachineComponent();
		}
		else
		{
			ST_Player1 = PlayerController->GetStateMachineComponent();
		}
	}
}

void AGameStateBaseCoop::SetZoneInfo(const EZoneType& Zone, const ELevelSide& Side, const FVector Location)
{
	ZoneType = Zone;
	LevelSide = Side;
	RespawnLoc = Location;
}

const AActor* AGameStateBaseCoop::GetPlayer(int Index) const
{
	if(Index == 0)
	{	
		return ST_Player0->GetCurrentActor();
	}
	else if(Index == 1)
	{
		return ST_Player1->GetCurrentActor();
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
			if(IsGameOver()){}
			else
			{
				if(NewPlayerState == EPlayerGlobalState::Alive)
				{
					ST_Player0->UpdateState(ReviveTag);
				}
				else
				{			
					ST_Player0->UpdateState(DeadTag);
				}
			}
		}
	}
	else if(PlayerIndex == 1)
	{	
		if(Player1GlobalState != NewPlayerState)
		{		
			Player1GlobalState = NewPlayerState;
			if(IsGameOver()){}
			else
			{
				if(NewPlayerState == EPlayerGlobalState::Alive)
				{
				
					ST_Player1->UpdateState(ReviveTag);
				}
				else
				{			
					ST_Player1->UpdateState(DeadTag);
				}
			}
		}
	}
}

bool AGameStateBaseCoop::IsGameOver() const
{
	bool bGameOver = false;

	if ((Player0GlobalState == EPlayerGlobalState::Dead && Player1GlobalState == EPlayerGlobalState::Dead) ||
		(Player0GlobalState == EPlayerGlobalState::Dead && ST_Player1->GetIACount() <= 1) ||
		(Player1GlobalState == EPlayerGlobalState::Dead && ST_Player0->GetIACount() <= 1))
	{
		bGameOver = true;
		ST_Player0->UpdateStateFromTransition(GameOverTag);
		ST_Player1->UpdateStateFromTransition(GameOverTag);

		FTimerHandle timer;
		GetWorldTimerManager().SetTimer(timer, FTimerDelegate::CreateLambda([&] { UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameOverMap); }), 1.0f, false);
		
	}
	return bGameOver;
}

