// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class COOPIA_API AShield : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	AShield();

	void SetOwner(class ACharacterBase* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void RotationLeftStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void RotationLeftCompleted();

	UFUNCTION(BlueprintImplementableEvent)
	void RotationRightStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void RotationRightCompleted();
	
	void Hide();
	void Show();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	float ShieldAngle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<class ACharacterBase> m_Player;

	void UpdateShieldLocation();
};
