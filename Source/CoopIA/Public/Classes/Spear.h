// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Enum/SpearState.h"
#include "Data/Interface/PlayerInteract.h"
#include "GameFramework/Character.h"
#include "Spear.generated.h"

class UInputMappingContext;
class UInputAction;
class UDASpear;
struct FInputActionValue;
struct FGameplayTagContainer;

UCLASS(config=Game)
class COOPIA_API ASpear : public ACharacter, public IPlayerInteract, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SpearMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FormationMappingContext;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* SpearMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Neutral Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NeutralAction;

	/** Shield Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShieldAction;
    	
	/** Ball Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BallAction;

	/** DataAsset Spear */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UDASpear* DASpear;

public:
	// Sets default values for this character's properties
	ASpear();
	
	void SetAIManager(class AAIManager* Manager);

	void Hide();
	void Show();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	
	void StartHold();
	
	void DashUp();
	void DashForward();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	//Interfaces
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override {TagContainer = ActorTags; };

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;
	
	bool bCanUpdateTimer;
	bool bCanDash = true;
	bool bStartHold;
	
	float HoldTimer;

	FTimerHandle TimerDashForward;
	FTimerHandle TimerDash;

	ESpearState SpearState = ESpearState::STATIC;

	TObjectPtr<class AAIManager> AIManager;
	
	void StartBall();
	void StartNeutral();
	void StartShield();
};
