// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBaseIA.generated.h"

UCLASS()
class COOPIA_API ACharacterBaseIA : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBaseIA();

	void Move(FVector Destination, bool bIsTransition, float AcceptanceRadius);

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 PlayerIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void Hide();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY()
	TObjectPtr<class AAIManager> Manager;

	UPROPERTY(EditAnywhere, category = "c++", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDA_IA> DataAssetIA;
};
