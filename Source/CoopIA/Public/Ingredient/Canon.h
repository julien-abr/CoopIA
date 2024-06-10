// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Canon.generated.h"

class UDABall;
class IInteract;

UCLASS()
class COOPIA_API ACanon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _cube;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* _box;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> _arrowL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> _arrowR;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDABall> DABall;
	
public:	
	// Sets default values for this actor's properties
	ACanon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/*UPROPERTY()
	TScriptInterface<IInteract> InteractInterfaceP1;
	UPROPERTY()
	TScriptInterface<IInteract> InteractInterfaceP2;*/

	UPROPERTY()
	TObjectPtr<AActor> BouleToLaunchP1;
	UPROPERTY()
	TObjectPtr<AActor> BouleToLaunchP2;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> BoulesToLaunch;
	

	UPROPERTY()
	TArray<UArrowComponent*> Arrows;

	FTimerHandle TriggerLaunchTimerHandle;

	UPROPERTY(EditAnywhere)
	float launchForce;
	UPROPERTY(EditAnywhere)
	float maxVelocityBoule;

	UFUNCTION()
	void TriggerLaunch();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
