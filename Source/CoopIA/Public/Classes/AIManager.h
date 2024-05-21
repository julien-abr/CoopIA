// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

enum class EIAState;

UCLASS()
class COOPIA_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

	void Init(class ACharacterBase* Character, class AMainCamera* Camera);
	
	void AddPlayer(class ACharacterBaseIA* IA);

	void IASucceededTransition();

	void UpdateState(const EIAState& State);

	TArray<ACharacterBaseIA*> SplitAI();

	FVector FindLastPos();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 ManagerIndex;

	const AActor* GetCurrentActor() const
	{
		return CurrentActor;
	}

private:
	UFUNCTION()
	void IARandomMove();

	void FindLastHex();

	void Spear(EIAState State);

	void Shield(EIAState State);

	void Ball(EIAState State);

	void Neutral(EIAState State);

	void HidePrevious(EIAState State);

	void PlayerDied(EIAState State);
	
	void PlayerRevived(EIAState State);

	void InitIA();

	FTransform GetTransfoPos(EIAState State);
	FVector GetLastPos(EIAState State);

	void TeleportIA();
	void TeleportIAFailed(ACharacterBaseIA* IA, FVector PlayerLoc, float DestinationZ);
	
	UPROPERTY(EditAnywhere, category = "c++", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDA_IA> DataAssetIA;

	UPROPERTY(EditAnywhere, category = "Formation", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ASpear> SpearBP;

	UPROPERTY(EditAnywhere, category = "Formation", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AShield> ShieldBP;

	UPROPERTY(EditAnywhere, category = "Formation", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ABall> BallBP;

	UPROPERTY()
	TObjectPtr<AAIManager> OtherManager;
	
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
	TObjectPtr<class APlayerControllerBase> PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ACharacterBaseIA>> ArrayIA;

	UPROPERTY()
	TObjectPtr<AMainCamera> MainCamera;
	
	FVector PlayerLastHexPos;
	
	UPROPERTY()
	TObjectPtr<class UNavigationSystemV1> NavSystem;

	EIAState IAState;
	EIAState PreviousState;

	FTimerHandle Handle;
	FTimerHandle HandleHexRaycast;

	int8 NumberIAToSucceed;

	bool bIsInTransition;
};
