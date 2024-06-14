// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransitionEffect.generated.h"

UCLASS()
class COOPIA_API ATransitionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransitionEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void Init(const int playerIndex, const FVector& effectLoc, const int duration);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
