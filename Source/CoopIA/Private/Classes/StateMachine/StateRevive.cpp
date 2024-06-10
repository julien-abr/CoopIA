// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateRevive.h"

#include "Classes/GameStateBaseCoop.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "Classes/PlayerControllerBase.h"

//Libraries
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Kismet/GameplayStatics.h"

void UStateRevive::OnStateEnter(UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	
	UE_LOG(LogTemp, Warning, TEXT("Enter Revive"));
	ST->HidePrevious();
	ST->PlayerController->UnPossess();
	ST->PlayerController->SetControlRotation(FRotator());

	//TODO::Puzzle or Other Map
	AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState && GameState->GetZoneType() == EZoneType::Puzzle)
	{
		ST->Player->SetActorLocation(GameState->GetRespawnLoc(), false, nullptr, ETeleportType::TeleportPhysics);
	}	
	ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	ST->PlayerController->Possess(ST->Player);
	ST->Player->Revive();
	
	ST->ArrayIA = ST->OtherST->SplitAI();
	//InitIA();
	ST->CurrentActor = ST->Player;
	ST->MainCamera->SetPlayer(ST->Player, ST->PlayerIndex);

	//Go to random move
	ST->UpdateState(ST->DA_StateMachine->NeutralState);
}

void UStateRevive::OnStateTick()
{
	Super::OnStateTick();
}

void UStateRevive::OnStateLeave()
{
	Super::OnStateLeave();
}
