// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "WindCurrent.generated.h"

UCLASS()
class COOPIA_API AWindCurrent : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _cube;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _cylinder;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* _box;
	
public:	
	// Sets default values for this actor's properties
	AWindCurrent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float windForce;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY()
	bool inTheCurrent;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
