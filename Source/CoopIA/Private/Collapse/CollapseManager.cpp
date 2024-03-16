// Fill out your copyright notice in the Description page of Project Settings.


#include "Collapse/CollapseManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
#include "Misc/DefaultValueHelper.h"

// Sets default values
ACollapseManager::ACollapseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollapseManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollapseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACollapseManager::CheckHexExist(FVector hexPos)
{
	return _hexBuildMap.Contains(GenerateHexKey(hexPos));
}

void ACollapseManager::GetAllHex()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _hexaClass, foundActors);

	if (foundActors.Num() == _hexBuildMap.Num())
		return;

	for (int i = 0; i < foundActors.Num(); i++)
	{
		FIntVector2 key = GenerateHexKey(foundActors[i]->GetActorLocation());

		if(!_hexBuildMap.Contains(key))
		{
			AddNewHex(foundActors[i]);
#if WITH_EDITOR
			foundActors[i]->SetFolderPath("Level_Hex");
#endif
		}
		else if(foundActors[i] != _hexBuildMap[key])
		{
			foundActors[i]->Destroy();
		}
	}
}

void ACollapseManager::AddNewHex(AActor* newHex)
{
	FVector hexPos = newHex->GetActorLocation();
	FIntVector2 key = GenerateHexKey(hexPos);

	_hexBuildMap.Add(key, newHex);

	int lineKey = UKismetMathLibrary::Round(hexPos.X);

	if (!_hexLineMap.Contains(lineKey))
		_hexLineMap.Add(lineKey, FHexArray());

	_hexLineMap[lineKey]._hexArray.Add(newHex);
}

void ACollapseManager::ClearDeletedHex()
{
	for(auto it = _hexBuildMap.CreateIterator(); it; ++it)
	{
		if(!it.Value())
		{
			_hexLineMap[it.Key().X]._hexArray.RemoveAll([](AActor* actor)
			{
				return actor == nullptr;
			});

			it.RemoveCurrent();
		}
	}
}

FIntVector2 ACollapseManager::GenerateHexKey(FVector hexPos)
{
	int x = UKismetMathLibrary::Round(hexPos.X);
	int y = UKismetMathLibrary::Round(hexPos.Y);

	return FIntVector2(x, y);
}

