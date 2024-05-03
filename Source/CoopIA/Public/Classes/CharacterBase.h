// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIManager.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Interface/Interact.h"
#include "Data/Interface/PlayerInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UInputMappingContext;
class UInputAction;
class UDAPlayer;
class UDAShield;
struct FInputActionValue;
struct FGameplayTagContainer;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacterBase, Log, All);

UCLASS(config=Game)
class COOPIA_API ACharacterBase : public ACharacter, public IPlayerInterface, public IInteract, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FormationMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* ShieldMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DeadMappingContext;

	/** Ball Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShieldMoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Neutral Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NeutralAction;
	
    /** Spear Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* SpearAction;
    	
    /** Ball Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* BallAction;

	/** Shield Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShieldAction;

	/** Dead Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DeadImpulseAction;
    
    /** Shield Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* ShieldRotateLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShieldRotateRightAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> MaterialDead0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> MaterialDead1;

public:
	// Sets default values for this character's properties
	ACharacterBase();

	void Init(class AAIManager* Manager);

	void SetupShield(class AShield* Shield);
	void DeactivateShield();

	void Hide();

	void Show();

	void Died();
	
	void Revive();

	UFUNCTION()
	void ImpulseTowardActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//Interfaces
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override {TagContainer = ActorTags; };

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Interface IPlayerInterface
	virtual EIAState GetAIState_Implementation() override;

	virtual int32 GetPlayerIndex_Implementation() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDAPlayer> DAPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDAShield> DAShield;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannelDead;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannelAlive;
	
	TObjectPtr<class AAIManager> AIManager;
	TObjectPtr<class AShield> ShieldActor;

	bool bIsShieldActivate;

	void SetupDefaultMapping();

	void SetupDeadMapping();

	void StartSpear();
	void StartBall();
	void StartShield();
	void StartNeutral();
	
	void ShieldRotateLeftStarted();
	void ShieldRotateRightStarted();

	void ShieldRotateLeftCompleted();
	void ShieldRotateRightCompleted();
	bool isBind;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialAlive0;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialAlive1;
};
