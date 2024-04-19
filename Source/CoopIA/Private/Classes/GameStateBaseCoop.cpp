// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/GameStateBaseCoop.h"

#include "Classes/DeathManager.h"
#include "Kismet/GameplayStatics.h"

AGameStateBaseCoop::AGameStateBaseCoop()
{
	//Get DeathManager and Subscribe to the Delegate
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADeathManager::StaticClass());
	ADeathManager* DeathManager = Cast<ADeathManager>(Actor);

	if(DeathManager)
	{
		DeathManager->OnPlayerGlobalStateChangedDelegate.AddUObject(this, &AGameStateBaseCoop::OnPlayerGlobalStateChanged);
	}
}

void AGameStateBaseCoop::OnPlayerGlobalStateChanged(int32 PlayerIndex, EPlayerGlobalState NewPlayerState)
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate called in GameState"));

	if(PlayerIndex == 0)
	{
		Player0GlobalState = NewPlayerState;
	}
	else if(PlayerIndex == 1)
	{
		Player1GlobalState = NewPlayerState;
	}

	CheckGameOver();
	
}

void AGameStateBaseCoop::CheckGameOver()
{
	if(Player0GlobalState == EPlayerGlobalState::Dead && Player1GlobalState == EPlayerGlobalState::Dead)
	{
		//Play game over menu
	}
}
