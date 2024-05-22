// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PressurePlate.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API APressurePlate : public ATriggerBox
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* body;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	APressurePlate();

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	bool isUnique;

	UPROPERTY(EditAnywhere)
	bool makeObjectAppear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> object;
	
private:
	TArray<AActor*> actorList;
};
