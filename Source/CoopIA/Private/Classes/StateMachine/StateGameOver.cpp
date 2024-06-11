// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateGameOver.h"

#include "Classes/CharacterBase.h"
#include "Classes/GameStateBaseCoop.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "Kismet/GameplayStatics.h"

void UStateGameOver::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	
	UE_LOGFMT(LogTemp, Warning, "Enter GameOver - #{0}", ST->PlayerIndex);
	ST->HidePrevious();
	ST->DestroyIA();
	ST->MainCamera->SetPlayer(nullptr, ST->PlayerIndex);
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

	
}

void UStateGameOver::OnStateTick()
{
	Super::OnStateTick();
}

void UStateGameOver::OnStateLeave()
{
	Super::OnStateLeave();
}