// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateShield.h"

#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Shield.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "CoopIA/CoopIAGameMode.h"

void UStateShield::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE SHIELD"));
	
	const FGameplayTag& PreviousStateTag = ST->GetLastTagTransitionExcluded();

	ST->Player->Show();
	if(PreviousStateTag != FGameplayTag::EmptyTag && PreviousStateTag != ST->DA_StateMachine->NeutralState)
	{
		bHasInitNeutral = true;
		UE_LOG(LogTemp, Warning, TEXT("Enter STATE SHIELD Not from Neutral"));
		ST->PlayerController->UnPossess();
		ST->PlayerController->SetControlRotation(FRotator());
		const FVector Destination = ST->GetPositionForState();

		ST->Player->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
		ST->Player->SetActorRelativeRotation(ST->Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
		ST->CurrentActor = ST->Player;
		ST->MainCamera->SetPlayer(ST->Player, ST->PlayerIndex);
		ST->PlayerController->Possess(ST->Player);
	}

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE SHIELD from Neutral"));
	ST->HideIA();

	//UI
	if(ST->GameMode)
		ST->GameMode->UpdateUI(ST->PlayerIndex, ST->DA_StateMachine->ShieldTexture);
	ST->Player->SetupShield(ST->ShieldActor);
}

void UStateShield::OnStateTick()
{
	Super::OnStateTick();
}

void UStateShield::OnStateLeave()
{
	Super::OnStateLeave();
	if(bHasInitNeutral)
	{
		ST->OnHidePrevious.AddUObject(ST, &UStateMachineComponent::HideNeutral);
		ST->OnHidePrevious.AddUObject(ST, &UStateMachineComponent::HideShield);
	}
	else
	{
		ST->OnHidePrevious.AddUObject(ST, &UStateMachineComponent::HideShield);
	}
	
	ST->Player->DeactivateShield();
	UE_LOG(LogTemp, Warning, TEXT("SHIELD => OK"));
}