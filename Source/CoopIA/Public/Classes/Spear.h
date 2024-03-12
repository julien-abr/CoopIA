// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Data/Enum/SpearState.h"
#include "GameFramework/Character.h"
#include "Spear.generated.h"

class UInputMappingContext;
class UInputAction;
class UDASpear;
struct FInputActionValue;

UCLASS(config=Game)
class COOPIA_API ASpear : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SpearMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContextToRemove;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** DataAsset Spear */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UDASpear* DASpear;

public:
	// Sets default values for this character's properties
	ASpear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	
	void StartHold();
	
	void DashUp();
	void DashForward();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	bool bCanUpdateTimer;
	bool bCanDash = true;

	bool bStartHold;
	
	float HoldTimer;

	FTimerHandle TimerDashForward;
	FTimerHandle TimerDash;

	ESpearState SpearState = ESpearState::STATIC;
};
