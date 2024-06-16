// Fill out your copyright notice in the Description page of Project Settings.


#include "Collapse/CollapseManager.h"

#include "GameplayTagAssetInterface.h"
#include "Classes/DeathManager.h"
#include "Data/Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
#include "Tool/HexBehaviour.h"

// Sets default values
ACollapseManager::ACollapseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_arrowPoint = CreateDefaultSubobject<UArrowComponent>("Point");
	_arrowPoint->SetupAttachment(RootComponent);

	collapseDeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));
	collapseDeathZone->SetupAttachment(_arrowPoint);
}

// Called when the game starts or when spawned
void ACollapseManager::BeginPlay()
{
	Super::BeginPlay();

	collapseDeathZone->OnComponentBeginOverlap.AddDynamic(this, &ACollapseManager::OnBoxBeginOverlap);

	GetAllHex();

	//Activate Collapse
	if(!isCollapseOn || _hexLineMap.IsEmpty())
		return;

	ClearDeletedHex();

	SortPuzzleZone();

	collaspeActorArray.RemoveAll([](AActor* actor){ return actor == nullptr; });
	if (!collaspeActorArray.IsEmpty())
		collaspeActorArray.Sort([](const AActor& a, const AActor& b) { return a.GetActorLocation().X < b.GetActorLocation().X; });

	AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
	_gameStateCoop = Cast<AGameStateBaseCoop>(gameState);

	//Get First Key
	auto it = _hexLineMap.CreateIterator();
	_key = it->Key - 450;

	NextKey();

	ADeathManager* deathManager = Cast<ADeathManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADeathManager::StaticClass()));
	if(deathManager)
	{
		deathManager->SetDeathManagerHight(-4000.f);
		yPos = deathManager->GetActorLocation().Y;
	}
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexBehaviour::StaticClass(), foundActors);
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
	_key += 450; //450 = distance btw each Hex on X

	SetActorLocation(FVector(_key, yPos, -2000));

	//Go Next
	if(!_hexLineMap.Contains(_key))
	{
		GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::NextKey, voidTime, false);
		return;
	}

	bool inPuzzle = false;
	//For Each puzzle zone
	for(auto it = puzzleZoneArray.CreateIterator(); it; ++it)
	{
		//Zone exceed
		if (it->startZone < _key)
		{
			it.RemoveCurrent();
			continue;
		}

		//Zone far away
		if (it->startZone > _key)
			break;

		//Zone actual
		if(it->startZone == _key)
		{
			//Not on the good side, remove it
			if(_gameStateCoop->GetLevelSide() != it->side)
			{
				it.RemoveCurrent();
				continue;
			}

			//Take all line
			while(it->endZone + 450 > _key)
			{
				if(_hexLineMap.Contains(_key))
				{
					auto it2 = _hexLineMap.CreateIterator();
					_puzzleHexArray.Append(it2->Value.hexArray);
					it2.RemoveCurrent();
				}

				_key += 450;
			}

			//Because we have exceed the key by one line
			_key -= 450;

			inPuzzle = true;
			GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::FirstPreventCollapseLine, it->duration - startOfCollaspeTime, false);
			it.RemoveCurrent();
			return;
		}
	}

	if (inPuzzle)
		return;

	//If no puzzle zone, take next line
	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::FirstPreventCollapseLine, hexTotalLifeTime - startOfCollaspeTime, false);
}

void ACollapseManager::FirstPreventCollapseLine()

{
	if (_hexLineMap.IsEmpty())
		return;

	OnCorruptionStart();

	//If puzzle zone exist
	if(!_puzzleHexArray.IsEmpty())
	{
		for (int i = 0; i < _puzzleHexArray.Num(); i++)
		{
			if(_puzzleHexArray[i])
				_puzzleHexArray[i]->FirstPreventAnim();
		}
	}
	//Else, juste take the next hex line
	else
	{
		auto it = _hexLineMap.CreateIterator();

		for (int i = 0; i < it->Value.hexArray.Num(); i++)
		{
			if (it->Value.hexArray[i])
				it->Value.hexArray[i]->FirstPreventAnim();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::SecondPreventCollapseLine, startOfCollaspeTime / 2, false);
}

void ACollapseManager::SecondPreventCollapseLine()
{
	if (_hexLineMap.IsEmpty())
		return;

	//If puzzle zone exist
	if (!_puzzleHexArray.IsEmpty())
	{
		for (int i = 0; i < _puzzleHexArray.Num(); i++)
		{
			if(_puzzleHexArray[i])
				_puzzleHexArray[i]->SecondPreventAnim();
		}
	}
	//Else, juste take the next hex line
	else
	{
		auto it = _hexLineMap.CreateIterator();

		for (int i = 0; i < it->Value.hexArray.Num(); i++)
		{
			if (it->Value.hexArray[i])
				it->Value.hexArray[i]->SecondPreventAnim();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(_collapseTimer, this, &ACollapseManager::CollapseLine, startOfCollaspeTime / 2, false);
}

void ACollapseManager::CollapseLine()
{
	if(_hexLineMap.IsEmpty())
		return;

	//If puzzle zone exist
	if (!_puzzleHexArray.IsEmpty())
	{
		for (int i = 0; i < _puzzleHexArray.Num(); i++)
		{
			if(_puzzleHexArray[i])
				_puzzleHexArray[i]->FallAnim();
		}

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

	if(!collaspeActorArray.IsEmpty())
	{
		for (auto it = collaspeActorArray.CreateIterator(); it; ++it)
		{
			if(!collaspeActorArray[it.GetIndex()]) {continue;}

			//Zone exceed
			if (collaspeActorArray[it.GetIndex()]->GetActorLocation().X < _key)
			{
				FVector pos = collaspeActorArray[it.GetIndex()]->GetActorLocation() + FVector(0, 0, -5000.f);

				collaspeActorArray[it.GetIndex()]->GetRootComponent()->SetMobility(EComponentMobility::Movable);
				FLatentActionInfo latentInfoMesh;
				latentInfoMesh.CallbackTarget = collaspeActorArray[it.GetIndex()];
				UKismetSystemLibrary::MoveComponentTo(collaspeActorArray[it.GetIndex()]->GetRootComponent(), pos, collaspeActorArray[it.GetIndex()]->GetActorRotation(), false, true, 5.f, true, EMoveComponentAction::Move, latentInfoMesh);

				it.RemoveCurrent();
				continue;
			}

			//Zone far away
			if (collaspeActorArray[it.GetIndex()]->GetActorLocation().X > _key)
				break;
		}
	}

	NextKey();
}

void ACollapseManager::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isCollapseOn)
		return;

	if (OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[COLLASPE] Entity Hit Zone : %s"), *OtherActor->GetName());
		const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(OtherActor);
		FGameplayTagContainer OtherActorTag;
		Interface->GetOwnedGameplayTags(OtherActorTag);
		if (OtherActorTag.HasTag(PlayerTag))
		{
			if (OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
			{
				UE_LOGFMT(LogTemp, Warning, "[COLLASPE] Player hit CollaspeZone");
				if(_gameStateCoop)
					_gameStateCoop->GameOver();
			}
		}
	}
}

//TOOL ONLY
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

	puzzleZoneArray = puzzleZoneList;

	for(int i = 0; i < puzzleZoneArray.Num(); i++)
	{
		if(!puzzleZoneArray[i].watchOnMap)
			continue;

		TArray<int> mapKey;
		_hexLineMap.GetKeys(mapKey);

		for (int j = 0; j < mapKey.Num(); j++)
		{
			if (mapKey[j] >= puzzleZoneArray[i].startZone && mapKey[j] <= puzzleZoneArray[i].endZone)
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
	if(!puzzleZoneArray.IsEmpty())
		puzzleZoneArray.Sort([](FPuzzleZoneData a, FPuzzleZoneData b) { return a.startZone < b.startZone; });
}
TArray<FPuzzleZoneData> ACollapseManager::GetPuzzleZone()
{
	return puzzleZoneArray;
}

FIntVector2 ACollapseManager::GenerateHexKey(FVector hexPos)
{
	int x = UKismetMathLibrary::Round(hexPos.X);
	int y = UKismetMathLibrary::Round(hexPos.Y);

	return FIntVector2(x, y);
}
