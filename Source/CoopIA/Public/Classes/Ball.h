// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Interface/PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

class UInputMappingContext;
class UInputAction;
struct FGameplayTagContainer;
class UStateMachineComponent;

UCLASS()
class COOPIA_API ABall : public APawn, public IGameplayTagAssetInterface, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FormationMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* BallMappingContext;

	/** Ball Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BallMoveAction;

	/** Spear Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NeutralAction;
	
	/** Shield Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShieldAction;
    	
	/** Spear Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpearAction;

public:
	// Sets default values for this pawn's properties
	ABall();

	UFUNCTION(BlueprintImplementableEvent)
	void SetSimulatePhysics(const bool enable);
	
	void Hide();
	void Show();

	virtual int32 GetPlayerIndex_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Interfaces
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override {TagContainer = ActorTags; };

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;

	UPROPERTY()
	TObjectPtr<UStateMachineComponent> ST;
	
	TObjectPtr<class ST> AIManager;

	UPROPERTY(EditAnywhere)
	FGameplayTag SpearTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag NeutralTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag ShieldTag;
	
	UFUNCTION(BlueprintCallable)
		bool CheckIsFalling();

	void StartSpear();
	void StartNeutral();
	void StartShield();
};
