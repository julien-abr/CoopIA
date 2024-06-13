// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateNeutral.h"

#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/StateMachine/StateMachineComponent.h"

void UStateNeutral::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE NEUTRAL"));
	ST->PlayerController->UnPossess();
	ST->PlayerController->SetControlRotation(FRotator());
	const FVector Destination = ST->GetPositionForState();

	ST->Player->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
	ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	ST->Player->Show();

	ST->CurrentActor = ST->Player;
	ST->MainCamera->SetPlayer(ST->Player, ST->PlayerIndex);
	ST->PlayerController->Possess(ST->Player);
	
	ST->ShowAndTeleportIA();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&] { ST->IARandomMove();}), ST->DA_StateMachine->DataAssetIA->RandomMoveTime, true);
}

void UStateNeutral::OnStateTick()
{
	Super::OnStateTick();
}

void UStateNeutral::OnStateLeave()
{
	Super::OnStateLeave();
	ST->OnHidePrevious.BindLambda([&]{ST->Hide(ST->DA_StateMachine->NeutralState);});
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	UE_LOG(LogTemp, Warning, TEXT("NEUTRAL => OK"));
}