// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IntVectorTypes.h"
#include "GameFramework/Actor.h"
#include "CollapseManager.generated.h"

USTRUCT(BlueprintType)
struct FHexArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> _hexArray;
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

	UFUNCTION(BlueprintCallable)
	bool CheckHexExist(FVector hexPos);

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _hexaClass;

	UFUNCTION(BlueprintCallable)
	void GetAllHex();

	UFUNCTION(BlueprintCallable)
	void AddNewHex(AActor* newHex);

	UFUNCTION(BlueprintCallable)
	void ClearDeletedHex();

private:
	UPROPERTY()
	TMap<FIntVector2, AActor*> _hexBuildMap;

	UPROPERTY()
	TMap<int, FHexArray> _hexLineMap;

	FIntVector2 GenerateHexKey(FVector hexPos);
};
