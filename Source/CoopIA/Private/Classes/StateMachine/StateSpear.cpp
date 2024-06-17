// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateSpear.h"

#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Spear.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "CoopIA/CoopIAGameMode.h"

void UStateSpear::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE SPEAR"));
	ST->HideIA();
	ST->PlayerController->UnPossess();
	ST->PlayerController->SetControlRotation(FRotator());

	const FVector Destination = ST->GetPositionForState();

	ST->SpearActor->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
	
	ST->SpearActor->Show();
	ST->CurrentActor = ST->SpearActor;
	ST->MainCamera->SetPlayer(ST->SpearActor, ST->PlayerIndex);
	ST->PlayerController->Possess(ST->SpearActor);
	
	//UI
	if(ST->GameMode)
		ST->GameMode->UpdateUI(ST->PlayerIndex, ST->DA_StateMachine->SpearTexture);
	ST->SpearActor->OnEnterSpear();
}

void UStateSpear::OnStateTick()
{
	Super::OnStateTick();
}

void UStateSpear::OnStateLeave()
{
	Super::OnStateLeave();
	ST->OnHidePrevious.BindLambda([&]{ST->Hide(ST->DA_StateMachine->SpearState);});
	UE_LOG(LogTemp, Warning, TEXT("SPEAR => OK"));
}