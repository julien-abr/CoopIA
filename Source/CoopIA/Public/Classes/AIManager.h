// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EIAState.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

UCLASS()
class COOPIA_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

	void Init(class ACharacterBase* Character);
	
	void AddPlayer(class ACharacterBaseIA* IA);

	void IASucceededTransition();

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

	void Spear();

	void Shield();

	void Ball();
	
	UPROPERTY(EditAnywhere, category = "c++", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDA_IA> DataAssetIA;
	
	UPROPERTY()
	TObjectPtr<class ACharacterBase> Player;

	UPROPERTY()
	TObjectPtr<class APlayerControllerBase> PlayerController;

	UPROPERTY()
	TArray<TObjectPtr<class ACharacterBaseIA>> ArrayIA;

	EIAState IAState = EIAState::RANDOM_MOVE;

	FTimerHandle Handle;

	int16 NumberIAToSucceed;

};
