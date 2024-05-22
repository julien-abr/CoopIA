// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Rails.generated.h"

class IInteract;

UCLASS()
class COOPIA_API ARails : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* _root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* _box;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _arrow;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _top;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _bottom;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _left;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _right;

public:	
	// Sets default values for this actor's properties
	ARails();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	IInteract* InteractInterface;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
