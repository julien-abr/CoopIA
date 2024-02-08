// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCamera.generated.h"

class UDACamera;
class UBillboardComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class COOPIA_API AMainCamera : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	UDACamera* DACamera;

	/** Billboard = empty (rootComponent) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* Billboard;

	/** SpringArm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
public:	
	// Sets default values for this actor's properties
	AMainCamera();

	void AddPlayer(AActor* Actor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CalculateCamMovement();

	void UpdateArmLength();

	
	UPROPERTY() 
	TArray<AActor*> m_arrayActors;
	
	UPROPERTY()
	TObjectPtr<AActor> m_currentPlayer;

	TArray<float> m_arrayDistances;
	
	FVector m_previousLoc;
};
