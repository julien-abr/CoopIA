// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Classes/StateMachine/State.h"
#include "DA_StateMachine.generated.h"

UCLASS()
class COOPIA_API UDA_StateMachine : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag DefaultState;

	UPROPERTY(EditAnywhere)
	FGameplayTag InitState;

	UPROPERTY(EditAnywhere)
	FGameplayTag NeutralState;

	UPROPERTY(EditAnywhere)
	FGameplayTag BallState;

	UPROPERTY(EditAnywhere)
	FGameplayTag SpearState;

	UPROPERTY(EditAnywhere)
	FGameplayTag ShieldState;

	UPROPERTY(EditAnywhere)
	FGameplayTag DeadState;

	UPROPERTY(EditAnywhere)
	FGameplayTag ReviveState;

	UPROPERTY(EditAnywhere)
	FGameplayTag TransitionState;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TSubclassOf<UState>> StatesMap;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDA_IA> DataAssetIA;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpear> SpearBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShield> ShieldBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABall> BallBP;
};
