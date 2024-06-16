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


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* body;

	APressurePlate();

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void MoveObjectAnim(bool IsDisappearing);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPressure(FVector pressurePos);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndPressure();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _fallAnimTime = 5.0f;

	UPROPERTY(EditAnywhere)
	bool isUnique;

	UPROPERTY(EditAnywhere)
	bool makeObjectDisappear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> object;
	
private:
	UPROPERTY()
	TArray<AActor*> actorList;

	FVector initialPos;

	FTimerHandle TriggerDisappearTimerHandle;
};
