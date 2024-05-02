// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Classes/GameStateBaseCoop.h>

#include "CoreMinimal.h"
#include "FPuzzleZoneData.h"
#include "GameFramework/Actor.h"
#include "CollapseManager.generated.h"

class AHexBehaviour;

USTRUCT(BlueprintType)
struct FHexArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AHexBehaviour*> hexArray;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCollapseOn = false;

	//Set Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float voidTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float startOfCollaspeTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hexTotalLifeTime = 5.f;

	UFUNCTION(BlueprintCallable)
	void GetAllHex();
private:
	UPROPERTY()
	TMap<int, FHexArray> _hexLineMap;
	UPROPERTY()
	TArray<FPuzzleZoneData> _puzzleZoneArray;

	UPROPERTY()
	TArray<AHexBehaviour*> _puzzleHexArray;

	FTimerHandle _collapseTimer;

	int _key;
	AGameStateBaseCoop* _gameStateCoop;

	void NextKey();
	void PreventCollapseLine();
	void CollapseLine();

	//TOOL ONLY
public :
	//CREATE HEX
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _oldHexaClass;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> _hexaClass;

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

	//PUZZLE
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMaterial> _overlayMat;

	UFUNCTION(BlueprintCallable)
	void UpdatePuzzleZone(TArray<FPuzzleZoneData> puzzleZoneList);
	UFUNCTION(BlueprintCallable)
	void ClearPuzzleZone();
	UFUNCTION(BlueprintCallable)
	void SortPuzzleZone();
	UFUNCTION(BlueprintCallable)
	TArray<FPuzzleZoneData> GetPuzzleZone();

private:
	//CREATE HEX
	UPROPERTY()
	TMap<FIntVector2, AActor*> _hexBuildMap;

	FIntVector2 GenerateHexKey(FVector hexPos);

	//PUZZLE
	UPROPERTY()
	TMap<int, FHexArray> _toolPuzzleHexLineMap;
};
