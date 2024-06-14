// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateInit.h"

#include "NavigationSystem.h"
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Shield.h"
#include "Classes/Spear.h"
#include "Classes/StateMachine/StateMachineComponent.h"

//Libraries
#include "Classes/Ball.h"
#include "Classes/UI/BallPlayerUI.h"
#include "Kismet/GameplayStatics.h"

void UStateInit::OnStateEnter(class UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	UE_LOG(LogTemp, Warning, TEXT("Enter STATE INIT"));
	
	//Set Player to be the pawn of the controller
	ST->Player = Cast<ACharacterBase>(ST->PlayerController->GetPawn());

	const UTexture2D* PlayerUI = (ST->PlayerIndex == 0) ? ST->DA_StateMachine->PlayerJ1 : ST->DA_StateMachine->PlayerJ2;
	ST->Player->UpdatePlayerCountUI(PlayerUI);
	
	//Set Current Actor
	ST->CurrentActor = ST->Player;
	
	ST->MainCamera = Cast<AMainCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCamera::StaticClass()));
	ST->NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	ST->PlayerLastHexPos = ST->CurrentActor->GetActorLocation();

	//SPEAR
	FActorSpawnParameters SpawnInfoSpear;
	SpawnInfoSpear.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ST->SpearActor = GetWorld()->SpawnActor<ASpear>(ST->DA_StateMachine->SpearBP, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoSpear);
	ST->SpearActor->Hide();
	ST->SpearActor->UpdatePlayerCountUI(PlayerUI);

	//SHIELD
	FActorSpawnParameters SpawnInfoShield;
	SpawnInfoShield.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ST->ShieldActor = GetWorld()->SpawnActor<AShield>(ST->DA_StateMachine->ShieldBP, ST->CurrentActor->GetActorLocation(), ST->CurrentActor->GetActorRotation(), SpawnInfoShield);
	ST->ShieldActor->SetOwner(ST->Player);
	ST->ShieldActor->SetActorRelativeRotation(FRotator(0, 0, 0));
	ST->ShieldActor->Hide();

	//BALL
	FActorSpawnParameters SpawnInfoBall;
	SpawnInfoBall.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ST->BallActor = GetWorld()->SpawnActor<ABall>(ST->DA_StateMachine->BallBP, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoBall);

	//Create BALL UI
	ABallPlayerUI* BallPlayerUI = GetWorld()->SpawnActor<ABallPlayerUI>(ST->DA_StateMachine->BallBP_UI, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoBall);
	BallPlayerUI->Init(ST->BallActor, PlayerUI);
	ST->BallActor->InitActorUI(BallPlayerUI);
	ST->BallActor->Hide();
	UE_LOG(LogTemp, Warning, TEXT("INIT => OK"));
	
	ST->UpdateState(ST->DA_StateMachine->NeutralState);
}

void UStateInit::OnStateTick()
{
	Super::OnStateTick();
}

void UStateInit::OnStateLeave()
{
	Super::OnStateLeave();
}