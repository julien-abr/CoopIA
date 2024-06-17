// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIManager.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Interface/Interact.h"
#include "Data/Interface/PlayerInterface.h"
#include "GameFramework/Character.h"
#include "StateMachine/StateMachineComponent.h"
#include "CharacterBase.generated.h"

class UInputMappingContext;
class UInputAction;
class UDAPlayer;
class UDAShield;
struct FInputActionValue;
struct FGameplayTagContainer;
class UDA_UI;
class AShield;
class UStateMachineComponent;

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

	void Init(UStateMachineComponent* StateMachineComponent);
	
	virtual int32 GetPlayerIndex_Implementation() override;

	const bool HasShieldActivate() const { return bIsShieldActivate;}

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerCountUI(const UTexture2D* Texture);

	void SetupShield(class AShield* Shield);
	void DeactivateShield();

	void Hide();

	void Show();

	void Died();
	
	void Revive() const;

	UFUNCTION()
	void ImpulseTowardActor();

	//Sound Event
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterNeutral();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveNeutral();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveShield(const int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void OnRevive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	//Interfaces
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override {TagContainer = ActorTags; };

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDAPlayer> DAPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDAShield> DAShield;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDA_UI> DA_UI;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannelDead;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannelAlive;

	UPROPERTY(EditAnywhere)
	FGameplayTag SpearTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag BallTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag ShieldTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag NeutralTag;

	UPROPERTY()
	TObjectPtr<UStateMachineComponent> ST;

	UPROPERTY()
	TObjectPtr<AShield> ShieldActor;



	bool bIsShieldActivate;

	void SetMaterial(bool bIsDead) const;

	void SetupDefaultMapping() const;

	void SetupDeadMapping() const;

	void StartSpear();
	void StartBall();
	void StartShield();
	void StartNeutral();
	
	void ShieldRotateLeftStarted();
	void ShieldRotateRightStarted();

	void ShieldRotateLeftCompleted();
	void ShieldRotateRightCompleted();
	bool isBind;

	UFUNCTION(BlueprintCallable)
	bool CheckIsFalling();

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialAlive0;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialAlive1;
};
