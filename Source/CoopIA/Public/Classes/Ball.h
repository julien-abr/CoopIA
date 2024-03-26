// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class COOPIA_API ABall : public APawn
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

	void SetAIManager(class AAIManager* Manager);
	
	void Hide();
	void Show();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	TObjectPtr<class AAIManager> AIManager;
	
	void StartSpear();
	void StartNeutral();
	void StartShield();
};
