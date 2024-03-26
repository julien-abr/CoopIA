// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollapseManager.generated.h"

class AHexBehaviour;

USTRUCT(BlueprintType)
struct FHexArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AHexBehaviour*> _hexArray;
};

UCLASS()
class COOPIA_API ACollapseManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollapseManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _oldHexaClass;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _hexaClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _isCollapseOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _hexLifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _preventHexLifeTime = 2.f;

	UFUNCTION(BlueprintCallable)
	bool CheckHexExist(FVector hexPos);

	UFUNCTION(BlueprintCallable)
	void UpdateAllOldHex();

	UFUNCTION(BlueprintCallable)
	void AddNewHex(AActor* newHex);

	UFUNCTION(BlueprintCallable)
	void ClearDeletedHex();

	UFUNCTION(BlueprintCallable)
	void SortLineMap();

private:
	UPROPERTY()
	TMap<FIntVector2, AActor*> _hexBuildMap;

	UPROPERTY()
	TMap<int, FHexArray> _hexLineMap;

	FTimerHandle _collapseTimer;

	FIntVector2 GenerateHexKey(FVector hexPos);

	void PreventCollapseLine();

	void CollapseLine();
};
