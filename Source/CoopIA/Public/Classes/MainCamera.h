// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCamera.generated.h"

class UDACamera;
class UBillboardComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ECameraState : uint8 
{
	FOLLOW = 0		 UMETA(DisplayName = "FOLLOW"),
	FIXED = 1        UMETA(DisplayName = "FIXED")
};

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
	
public:
	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	
public:	
	// Sets default values for this actor's properties
	AMainCamera();

    UFUNCTION(BlueprintCallable)
	 void AddPlayer(AActor* Actor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFixedPosition(FVector Position);

	void SetSpline(class ASpline* NewSpline);

private:
	void FollowPlayers();
	
	UPROPERTY() 
	TArray<AActor*> m_arrayActors;

	UPROPERTY(EditAnywhere, Category = "DefaultSpline", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> DefaultSpline;

	ECameraState CameraState = ECameraState::FOLLOW;
	
	UPROPERTY() 
	class ASpline* Spline;
};
