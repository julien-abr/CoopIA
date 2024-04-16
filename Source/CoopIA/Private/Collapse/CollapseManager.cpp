// Fill out your copyright notice in the Description page of Project Settings.


#include "Collapse/CollapseManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
#include "Misc/DefaultValueHelper.h"
#include "Tool/HexBehaviour.h"

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

	ClearDeletedHex();

	if(_isCollapseOn)
		GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::PreventCollapseLine, _preventHexLifeTime, false);
}

// Called every frame
void ACollapseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollapseManager::UpdateAllOldHex()
{
	//Replace old hex by new

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _oldHexaClass, foundActors);
	for (int i = 0; i < foundActors.Num(); i++)
	{
		FVector pos = foundActors[i]->GetActorLocation();
		foundActors[i]->Destroy();
		GetWorld()->SpawnActor<AActor>(_hexaClass, pos, FRotator(0.f, 0.f, 0.f));
	}

	//Add new ex not in map for the moment

	foundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _hexaClass, foundActors);

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
			if(Cast<AHexBehaviour>(_hexBuildMap[key]))
			{
				foundActors[i]->Destroy();
			}
			else
			{
				AddNewHex(foundActors[i]);
#if WITH_EDITOR
				foundActors[i]->SetFolderPath("Level_Hex");
#endif
			}
		}
	}

	ClearDeletedHex();
}

bool ACollapseManager::CheckHexExist(FVector hexPos)
{
	return _hexBuildMap.Contains(GenerateHexKey(hexPos));
}

void ACollapseManager::AddNewHex(AActor* newHex)
{
	FVector hexPos = newHex->GetActorLocation();
	FIntVector2 key = GenerateHexKey(hexPos);

	_hexBuildMap.Add(key, newHex);

	int lineKey = UKismetMathLibrary::Round(hexPos.X);

	if (!_hexLineMap.Contains(lineKey))
		_hexLineMap.Add(lineKey, FHexArray());

	if(AHexBehaviour* hex = Cast<AHexBehaviour>(newHex))
	{
		_hexLineMap[lineKey]._hexArray.Add(hex);
	}
}

//Remove manually removed hexagons from the map 
void ACollapseManager::ClearDeletedHex()
{
	for(auto it = _hexBuildMap.CreateIterator(); it; ++it)
	{

		if(!it.Value())
		{
			if(_hexLineMap.Find(it.Key().X))
			{
				_hexLineMap[it.Key().X]._hexArray.RemoveAll([](AActor* actor)
				{
					return actor == nullptr;
				});

				if (_hexLineMap[it.Key().X]._hexArray.IsEmpty())
					_hexLineMap.Remove(it.Key().X);
			}

			it.RemoveCurrent();
		}
	}
}

void ACollapseManager::SortLineMap()
{
	_hexLineMap.KeySort([](int a, int b) { return a < b; });
}

FIntVector2 ACollapseManager::GenerateHexKey(FVector hexPos)
{
	int x = UKismetMathLibrary::Round(hexPos.X);
	int y = UKismetMathLibrary::Round(hexPos.Y);

	return FIntVector2(x, y);
}

void ACollapseManager::PreventCollapseLine()
{
	if (_hexLineMap.IsEmpty())
		return;

	auto it = _hexLineMap.CreateIterator();

	for (int i = 0; i < it->Value._hexArray.Num(); i++)
	{
		if (it->Value._hexArray[i])
			it->Value._hexArray[i]->LaunchPreventCollaspeAnim();
	}

	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::CollapseLine, _hexLifeTime, false);
}

void ACollapseManager::CollapseLine()
{
	if(_hexLineMap.IsEmpty())
		return;

	auto it = _hexLineMap.CreateIterator();

	for(int i = 0; i < it->Value._hexArray.Num(); i++)
	{
		if(it->Value._hexArray[i])
			it->Value._hexArray[i]->LaunchCollapseAnim();
	}

	it.RemoveCurrent();

	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::PreventCollapseLine, _preventHexLifeTime, false);
}

