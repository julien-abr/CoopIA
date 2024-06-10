// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateShield.h"

#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Shield.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Classes/StateMachine/StateMachineComponent.h"

void UStateShield::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE SHIELD"));
	
	const FGameplayTag& PreviousStateTag = ST->GetLastTagTransitionExcluded();
	const bool bShouldInitNeutral =  (PreviousStateTag != FGameplayTag::EmptyTag && PreviousStateTag != ST->DA_StateMachine->NeutralState);

	if(bShouldInitNeutral)
	{
		ST->HidePrevious();
		ST->PlayerController->UnPossess();
		ST->PlayerController->SetControlRotation(FRotator());
		const FVector Destination = ST->GetPositionForState();

		ST->Player->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
		ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
		ST->Player->Show();

		ST->CurrentActor = ST->Player;
		ST->MainCamera->SetPlayer(ST->Player, ST->PlayerIndex);
		ST->PlayerController->Possess(ST->Player);
	}

	ST->HideIA();
	ST->ShieldActor->Show();
	//TODO::Leave shield here and not in Hide
	ST->Player->SetupShield(ST->ShieldActor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&] { ST->IARandomMove();}), ST->DA_StateMachine->DataAssetIA->RandomMoveTime, true);
	UE_LOG(LogTemp, Warning, TEXT("SHIELD => OK"));
}

void UStateShield::OnStateTick()
{
	Super::OnStateTick();
}

void UStateShield::OnStateLeave()
{
	Super::OnStateLeave();
}