// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Interface/RayHit.h"
#include "GameFramework/Actor.h"
#include "LaserContainer.generated.h"

UCLASS()
class COOPIA_API ALaserContainer : public AActor, public IRayHit
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RayHitAction_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> _door;
};
