// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "WindCurrent.generated.h"

class IInteract;

UCLASS()
class COOPIA_API AWindCurrent : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _cube;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* _box;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _windDirection;
	
public:	
	// Sets default values for this actor's properties
	AWindCurrent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float windForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inTheCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* actorInWind;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	/*UPROPERTY()
	bool inTheCurrent;*/

	TArray<IInteract*> InteractInterfaces;
	//UStaticMeshComponent* playerMesh;
	//AActor* actorInWind;
	TArray<AActor*> actorsInWind;

	TArray<AActor*> shieldsInWind;

	float windCurrentAngle;
	float windShieldAngle;




	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
