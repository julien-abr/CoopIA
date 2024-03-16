// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuilderTool.generated.h"

class UArrowComponent;

UCLASS()
class COOPIA_API ABuilderTool : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _ground;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* _arrow;

public:	
	// Sets default values for this actor's properties
	ABuilderTool();

public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _hexaPrefab;

	UFUNCTION(BlueprintCallable)
	FVector HexCoord(const FVector& start, const FVector& end);

	UFUNCTION(BlueprintCallable)
	AActor* BuildHex(const FVector& pos);

	UFUNCTION(BlueprintCallable)
	void ChangeHeight(const float& height);

private:
	float _xSpacing = 450.f;
	float _ySpacing = 520.f;
	float _offset = 260.f;
	float _height = 0.f;
};
