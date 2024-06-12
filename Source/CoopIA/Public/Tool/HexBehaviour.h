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

	const FVector GetRespawnLoc() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _fallAnimTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor firstColorCorruption = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor secondColorCorruption = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> disolveMat;

	UStaticMeshComponent* GetMesh();

	void FirstPreventAnim();
	void SecondPreventAnim();
	void FallAnim();

	bool isDead()
	{
		return _isDead;
	}

private:
	FTimerHandle _preventTimer;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dMatFloor;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dMatGrass;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dMatFloorDisolve;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dMatGrassDisolve;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dMatTake;
	FLinearColor _colorInit;
	FLinearColor _colorTake;

	bool _mustDissolve;
	int _state;
	float _timer;

	bool _isDead = false;

	void ChangeMaterialColor(TObjectPtr<UMaterialInstanceDynamic> dynMat, FLinearColor color);
	void DissolveMaterial(TObjectPtr<UMaterialInstanceDynamic> dynMat);
};
