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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 ManagerIndex;
	
private:
	UFUNCTION()
	void IARandomMove();

	void Spear(EIAState State);

	void Shield(EIAState State);

	void Ball(EIAState State);

	void Neutral(EIAState State);

	void HidePrevious(EIAState State);

	FTransform GetTransfoPos(EIAState State);
	
	UPROPERTY(EditAnywhere, category = "c++", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDA_IA> DataAssetIA;

	UPROPERTY(EditAnywhere, category = "Formation", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ASpear> SpearBP;

	UPROPERTY(EditAnywhere, category = "Formation", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AActor> ShieldBP;
	
	UPROPERTY()
	TObjectPtr<class ACharacterBase> Player;

	UPROPERTY()
	TObjectPtr<class ASpear> SpearActor;

	UPROPERTY()
	TObjectPtr<class AShield> ShieldActor;

	UPROPERTY()
	TObjectPtr<class AActor> CurrentActor;

	UPROPERTY()
	TObjectPtr<class APlayerControllerBase> PlayerController;

	UPROPERTY()
	TArray<TObjectPtr<class ACharacterBaseIA>> ArrayIA;

	UPROPERTY()
	TObjectPtr<class AMainCamera> MainCamera;

	EIAState IAState;
	EIAState PreviousState;

	FTimerHandle Handle;

	int16 NumberIAToSucceed;

	

};
