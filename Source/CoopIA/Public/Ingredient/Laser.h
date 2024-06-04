// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _firePoint;

	UPROPERTY(EditAnywhere)
	UStaticMesh* _laserCylinder;

public:	
	// Sets default values for this actor's properties
	ALaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int reflexionNbr = 5;

	UPROPERTY(EditAnywhere)
	float laserSize = 0.1f;

private :
	UFUNCTION()
	void ReflectLaser(const FVector& start, const FVector& end, float rotZ, int count, AActor* actor);

	void HideAllLaser();

	TArray<UStaticMeshComponent*> _laserCylinderArray;
};
