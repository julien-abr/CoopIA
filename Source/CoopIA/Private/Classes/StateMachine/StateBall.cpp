// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateBall.h"

#include "Classes/Ball.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/StateMachine/StateMachineComponent.h"


void UStateBall::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);

	UE_LOG(LogTemp, Warning, TEXT("Enter STATE BALL"));

	ST->HideIA();
	ST->PlayerController->UnPossess();
	
	const FVector Destination = ST->GetPositionForState();
	
	ST->BallActor->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
	ST->BallActor->SetActorRelativeRotation(FRotator(), false, nullptr, ETeleportType::TeleportPhysics);
	ST->BallActor->Show();
	
	ST->CurrentActor = ST->BallActor;
	ST->MainCamera->SetPlayer(ST->BallActor, ST->PlayerIndex);
	ST->PlayerController->Possess(ST->BallActor);
}

void UStateBall::OnStateTick()
{
	Super::OnStateTick();
}

void UStateBall::OnStateLeave()
{
	Super::OnStateLeave();
	ST->OnHidePrevious.BindLambda([&]{ST->Hide(ST->DA_StateMachine->BallState);});
	UE_LOG(LogTemp, Warning, TEXT("BALL => OK"));
	ST->BallActor->SetCanBeEjected(true);
}