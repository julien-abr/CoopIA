// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateRevive.h"

#include "Classes/GameStateBaseCoop.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "Classes/PlayerControllerBase.h"

//Libraries
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UStateRevive::OnStateEnter(UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	
	UE_LOGFMT(LogTemp, Warning, "Enter Revive - #{0}", ST->PlayerIndex);
	ST->PlayerController->UnPossess();
	ST->PlayerController->SetControlRotation(FRotator());
	
	if (const AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(GetWorld())); GameState && GameState->GetZoneType() == EZoneType::Puzzle)
	{
		ST->Player->SetActorLocation(GameState->GetRespawnLoc(), false, nullptr, ETeleportType::TeleportPhysics);
	}
	
	ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	ST->PlayerController->Possess(ST->Player);
	ST->Player->Revive();
	
	if(ST->OtherST)
	{
		ST->ArrayIA = ST->OtherST->SplitAI();
	}
	
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
	ST->OnHidePrevious.AddUObject(ST, &UStateMachineComponent::HideRevive);
	UE_LOG(LogTemp, Warning, TEXT("REVIVE => OK"));
}
