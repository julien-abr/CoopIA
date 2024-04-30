// Fill out your copyright notice in the Description page of Project Settings.


#include "Collapse/CollapseManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
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
	GetAllHex();

	//Activate Collapse
	if(!isCollapseOn || _hexLineMap.IsEmpty())
		return;

	//GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::PreventCollapseLine, startOfCollaspeTime, false);

	//Get First Key
	auto it = _hexLineMap.CreateIterator();
	_key = it->Key - 450;

	NextKey();
}

// Called every frame
void ACollapseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollapseManager::GetAllHex()
{
	_hexLineMap.Empty();

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _hexaClass, foundActors);
	for (int i = 0; i < foundActors.Num(); i++)
	{
		FVector hexPos = foundActors[i]->GetActorLocation();
		FIntVector2 key = GenerateHexKey(hexPos);

		int lineKey = UKismetMathLibrary::Round(hexPos.X);

		if (!_hexLineMap.Contains(lineKey))
			_hexLineMap.Add(lineKey, FHexArray());

		if (AHexBehaviour* hex = Cast<AHexBehaviour>(foundActors[i]))
		{
			_hexLineMap[lineKey].hexArray.Add(hex);
		}
	}

	SortLineMap();
}

void ACollapseManager::NextKey()
{
	UE_LOGFMT(LogTemp, Log, "Next Key {0}", _key);

	_key += 450; //450 = distance btw each Hex on X

	if(!_hexLineMap.Contains(_key))
	{
		UE_LOGFMT(LogTemp, Log, "Nothing, go next");
		GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::NextKey, voidTime, false);
		return;
	}

	bool inPuzzle = false;
	//Pour chaque zone puzzle encore existante
	for(int i = 0; i < _puzzleZoneArray.Num(); i++)
	{
		if (_puzzleZoneArray[i].startZone > _key)
			break;

		if(_puzzleZoneArray[i].startZone == _key)
		{
			UE_LOGFMT(LogTemp, Log, "Puzzle Zone Detected");
			//Attention, check dans quel endroit nous sommes, pas possible d'avoir une autre zone en meme temps
			//Si ta une zone Left et une Mid, et que tu es Mid, vire celle de gauche dans la liste et retry la loop

			//Ici, je sais que la prochaine ligne a tomber est un puzzle, donc je dois stocker dans une liste (deja faites) tout les hex concernés
			while(_puzzleZoneArray[i].endZone > _key)
			{
				if(_hexLineMap.Contains(_key))
				{
					auto it = _hexLineMap.CreateIterator();
					_puzzleHexArray.Append(it->Value.hexArray);
					it.RemoveCurrent();
				}

				_key += 450;
			}
			UE_LOGFMT(LogTemp, Log, "Puzzle Timer");
			inPuzzle = true;
			GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::PreventCollapseLine, _puzzleZoneArray[i].duration - startOfCollaspeTime, false);
			return;
		}
	}

	if (inPuzzle)
		return;

	UE_LOGFMT(LogTemp, Log, "Line Timer");
	//Si pas de zone puzzle, loop classique
	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::PreventCollapseLine, hexTotalLifeTime - startOfCollaspeTime, false);
}

void ACollapseManager::PreventCollapseLine()
{
	UE_LOGFMT(LogTemp, Log, "Prevent Collapse");

	if (_hexLineMap.IsEmpty())
		return;

	//If puzzle zone exist
	if(!_puzzleHexArray.IsEmpty())
	{
		UE_LOGFMT(LogTemp, Log, "PUZZLE Prevent Collapse");
		for (int i = 0; i < _puzzleHexArray.Num(); i++)
			_puzzleHexArray[i]->PreventAnim();
	}
	//Else, juste take the next hex line
	else
	{
		UE_LOGFMT(LogTemp, Log, "LINE Prevent Collapse");
		auto it = _hexLineMap.CreateIterator();

		for (int i = 0; i < it->Value.hexArray.Num(); i++)
		{
			if (it->Value.hexArray[i])
				it->Value.hexArray[i]->PreventAnim();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::CollapseLine, startOfCollaspeTime, false);
}
void ACollapseManager::CollapseLine()
{
	UE_LOGFMT(LogTemp, Log, "Collapse Hex");

	if(_hexLineMap.IsEmpty())
		return;

	//If puzzle zone exist
	if (!_puzzleHexArray.IsEmpty())
	{
		for (int i = 0; i < _puzzleHexArray.Num(); i++)
			_puzzleHexArray[i]->FallAnim();

		_puzzleHexArray.Empty();
	}
	//Else, juste take the next hex line
	else
	{
		auto it = _hexLineMap.CreateIterator();

		for(int i = 0; i < it->Value.hexArray.Num(); i++)
		{
			if(it->Value.hexArray[i])
				it->Value.hexArray[i]->FallAnim();
		}

		it.RemoveCurrent();
	}

	NextKey();
}

//TOOL ONLY
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
		_hexLineMap[lineKey].hexArray.Add(hex);
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
				_hexLineMap[it.Key().X].hexArray.RemoveAll([](AActor* actor)
				{
					return actor == nullptr;
				});

				if (_hexLineMap[it.Key().X].hexArray.IsEmpty())
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

void ACollapseManager::UpdatePuzzleZone(TArray<FPuzzleZoneData> puzzleZoneList)
{
	ClearPuzzleZone();

	_puzzleZoneArray = puzzleZoneList;

	for(int i = 0; i < _puzzleZoneArray.Num(); i++)
	{
		if(!_puzzleZoneArray[i].watchOnMap)
			continue;

		TArray<int> mapKey;
		_hexLineMap.GetKeys(mapKey);

		for (int j = 0; j < mapKey.Num(); j++)
		{
			if (mapKey[j] >= _puzzleZoneArray[i].startZone && mapKey[j] <= _puzzleZoneArray[i].endZone)
			{
				_toolPuzzleHexLineMap.Add(mapKey[j], _hexLineMap[mapKey[j]]);

				for(int k = 0; k < _hexLineMap[mapKey[j]].hexArray.Num(); k++)
				{
					_hexLineMap[mapKey[j]].hexArray[k]->GetMesh()->SetOverlayMaterial(_overlayMat);
				}
			}
		}
	} 
}

void ACollapseManager::ClearPuzzleZone()
{
	if (!_toolPuzzleHexLineMap.IsEmpty())
	{
		for (auto it = _toolPuzzleHexLineMap.CreateIterator(); it; ++it)
		{
			for (int i = 0; i < it->Value.hexArray.Num(); i++)
			{
				if (it->Value.hexArray[i])
					it->Value.hexArray[i]->GetMesh()->SetOverlayMaterial(nullptr);
			}
		}

		_toolPuzzleHexLineMap.Empty();
	}

}
void ACollapseManager::SortPuzzleZone()
{
	if(!_puzzleZoneArray.IsEmpty())
		_puzzleZoneArray.Sort([](FPuzzleZoneData a, FPuzzleZoneData b) { return a.startZone < b.startZone; });
}
TArray<FPuzzleZoneData> ACollapseManager::GetPuzzleZone()
{
	return _puzzleZoneArray;
}

FIntVector2 ACollapseManager::GenerateHexKey(FVector hexPos)
{
	int x = UKismetMathLibrary::Round(hexPos.X);
	int y = UKismetMathLibrary::Round(hexPos.Y);

	return FIntVector2(x, y);
}
