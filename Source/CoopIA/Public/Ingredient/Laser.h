// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

class UArrowComponent;

UCLASS()
class COOPIA_API ALaser : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _body;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* _firePoint;

	// Sets default values for this actor's properties
	ALaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int reflexionNbr = 5;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideAllLaser(int count);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLaserPos(int count, FVector start, FVector end);

private :
	UFUNCTION()
	void ReflectLaser(const FVector& start, const FVector& end, float rotZ, int& count, AActor* actor);

	UPROPERTY()
	TArray<TObjectPtr<UNiagaraComponent>> _laserEffectArray;
};
