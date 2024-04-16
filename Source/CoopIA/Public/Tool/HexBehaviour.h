// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexBehaviour.generated.h"

class UArrowComponent;

UCLASS()
class COOPIA_API AHexBehaviour : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _arrowRoot;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _arrowMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _hexMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _hexCollider;

public:	
	// Sets default values for this actor's properties
	AHexBehaviour();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _fallAnimTime = 5.0f;

	void LaunchPreventCollaspeAnim();

	void LaunchCollapseAnim();

private:
	FTimerHandle _preventTimer;
	FTimerHandle _collapseTimer;

	float _minShake = -20.f;
	float _maxShake = 20.f;

	void PreventAnim();
	void FallAnim();
	void DestroyHex();
};
