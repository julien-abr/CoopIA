
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Classes/StateMachine/State.h"
#include "StateMachineComponent.generated.h"


class UDA_StateMachine;
class APlayerControllerBase;
class ACharacterBase;
class ACharacterBaseIA;
class ASpear;
class ABall;
class AShield;
class AMainCamera;
class UNavigationSystemV1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPIA_API UStateMachineComponent : public UActorComponent
{
	friend class UStateInit;
	friend class UStateBall;
	friend class UStateSpear;
	friend class UStateNeutral;
	friend class UStateDead;
	friend class UStateShield;
	friend class UStateRevive;
	friend class UStateTransition;
	
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDA_StateMachine> DA_StateMachine;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FGameplayTag> StateHistoric;

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Init(APlayerControllerBase* ControllerRef, UDA_StateMachine* DA_StateMachineRef);

	void GetOtherST();
	
	void UpdateState(const FGameplayTag& Tag);

	void UpdateStateFromTransition(const FGameplayTag& Tag);

	void AddPlayer(ACharacterBaseIA* IACharacter);
	
	void RemoveAI(ACharacterBaseIA* CharacterAI);

	TArray<ACharacterBaseIA*> SplitAI();

	//============== Getter ===================
	const AActor* GetCurrentActor() const;
	const int GetIACount() const;
	const FGameplayTag& GetCurrentState() const;
	const int GetPlayerIndex() const;
	
private:
	void InitializeState(const FGameplayTag& Tag);

	void AddToHistoric(const FGameplayTag& Tag);

	void HidePrevious() const;

	const FGameplayTag& GetLastTagTransitionExcluded() const;

	const FVector GetPositionForState() const;

	const FRotator GetRotationForState() const;

	void FindLastHex();

	void IARandomMove();

	void ShowAndTeleportIA();
	void ShowAndTeleportIAFailed(ACharacterBaseIA* IA, FVector PlayerLoc, float DestinationZ);

	void HideIA();

	void DestroyIA();

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UState> CurrentState;

	UPROPERTY(VisibleInstanceOnly)
	FGameplayTag CurrentStateTag;

	UPROPERTY()
	FGameplayTag NextTag = FGameplayTag::EmptyTag;

	//========== Controller that inherit the StateMachine ============
	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
	//========== State Actor's ============
	UPROPERTY()
	TObjectPtr<ACharacterBase> Player;

	UPROPERTY()
	TObjectPtr<ASpear> SpearActor;

	UPROPERTY()
	TObjectPtr<AShield> ShieldActor;

	UPROPERTY()
	TObjectPtr<ABall> BallActor;

	UPROPERTY()
	TObjectPtr<AActor> CurrentActor;
	
	UPROPERTY()
	TObjectPtr<AMainCamera> MainCamera;

	UPROPERTY()
	TObjectPtr<UNavigationSystemV1> NavSystem;

	//========== StateMachine of second Player ============
	UPROPERTY()
	TObjectPtr<UStateMachineComponent> OtherST;
	
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<ACharacterBaseIA>> ArrayIA;

	FVector PlayerLastHexPos;

	int PlayerIndex;
	
	FTimerHandle HandleHexRaycast;

	//int8 NumberIAToSucceed;
};
