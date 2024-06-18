// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateTransition.h"

#include "Classes/CharacterBase.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Classes/StateMachine/StateMachineComponent.h"
#include "Classes/UI/TransitionEffect.h"
#include "Classes/UI/TransitionEffectPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UStateTransition::OnStateEnter(UStateMachineComponent*& StateMachineComponentRef)
{
	Super::OnStateEnter(StateMachineComponentRef);
	UE_LOG(LogTemp, Warning, TEXT("Enter STATE TRANSITION"));

	if(ST->Player)
		ST->Player->GetCharacterMovement()->Velocity = FVector::ZeroVector;

	bool bSkipAnim = ST->GetLastTagTransitionExcluded() == ST->DA_StateMachine->ReviveState || ST->GetLastTagTransitionExcluded() == ST->DA_StateMachine->DeadState;
	
	if(bSkipAnim)
	{
		ST->UpdateStateFromTransition(ST->NextTag);
		return;
	}

	if(ST->NextTag != FGameplayTag::EmptyTag && ST->NextTag != ST->DA_StateMachine->NeutralState)
	{
		FActorSpawnParameters SpawnInfoEffect;
		SpawnInfoEffect.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ATransitionEffectPlayer* transitionPlayerEffect = GetWorld()->SpawnActor<ATransitionEffectPlayer>(ST->DA_StateMachine->TransitionEffectPlayerBP, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoEffect);
		transitionPlayerEffect->Init(ST->GetPlayerIndex(), ST->DA_StateMachine->TransitonTime);
		for(auto IA : ST->ArrayIA)
		{
			if (!IA) { continue; }

			IA->Hide();
			ATransitionEffect* transitionEffect = GetWorld()->SpawnActor<ATransitionEffect>(ST->DA_StateMachine->TransitionEffectBP, IA->GetActorLocation(), FRotator(), SpawnInfoEffect);
			transitionEffect->Init(ST->GetPlayerIndex(), ST->CurrentActor->GetActorLocation(), ST->DA_StateMachine->TransitonTime);
		}
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			//VFX HERE
			UE_LOG(LogTemp, Warning, TEXT("Play vfx transition"));
			ST->UpdateStateFromTransition(ST->NextTag);
		}), ST->DA_StateMachine->TransitonTime, false);
	}
	else
	{
		FActorSpawnParameters SpawnInfoEffect;
		SpawnInfoEffect.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ATransitionEffectPlayer* transitionPlayerEffect = GetWorld()->SpawnActor<ATransitionEffectPlayer>(ST->DA_StateMachine->TransitionEffectPlayerBP, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoEffect);
		transitionPlayerEffect->Init(ST->GetPlayerIndex(), ST->DA_StateMachine->TransitonTime);
		for(auto IA : ST->ArrayIA)
		{
			ATransitionEffect* transitionEffect = GetWorld()->SpawnActor<ATransitionEffect>(ST->DA_StateMachine->TransitionEffectBP, ST->CurrentActor->GetActorLocation(), FRotator(), SpawnInfoEffect);
			transitionEffect->Init(ST->GetPlayerIndex(), ST->CurrentActor->GetActorLocation(), ST->DA_StateMachine->TransitonTime);
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			//Transition on ourself => go high and fall to the player
			UE_LOG(LogTemp, Warning, TEXT("No vfx ON IA transition"));
			ST->UpdateStateFromTransition(ST->NextTag);
		}), ST->DA_StateMachine->TransitonTime, false);
	}
}

void UStateTransition::OnStateTick()
{
	Super::OnStateTick();
}

void UStateTransition::OnStateLeave()
{
	Super::OnStateLeave();
	if (ST->OnHidePrevious.IsBound())
	{
		ST->OnHidePrevious.Execute();
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	UE_LOG(LogTemp, Warning, TEXT("TRANSITION => OK"));
}
