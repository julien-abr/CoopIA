// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/AIManager.h"
#include "Classes/Data/EIAState.h"
#include "Classes/AIControllerBase.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/CharacterBase.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/Spear.h"

//Library
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	IAState = EIAState::RANDOM_MOVE;
}

void AAIManager::Init(ACharacterBase* Character)
{
	Player = Character;
	PlayerController = Cast<APlayerControllerBase>(Player->GetController());
}

void AAIManager::AddPlayer(class ACharacterBaseIA* IA)
{
    ArrayIA.Add(IA);
}

void AAIManager::IASucceededTransition()
{
	NumberIAToSucceed--;
	//All IA Moved to the destination
	if(NumberIAToSucceed == 0)
	{
		switch(IAState)
		{
			case EIAState::BALL:
				Ball();
				break;
			case EIAState::SPEAR:
				Spear();
				break;
			case EIAState::SHIELD:
				Shield();
				break;
		}
	}
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AAIManager::IARandomMove, DataAssetIA->RandomMoveTime, true);
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIManager::IARandomMove()
{
	if(IAState != EIAState::RANDOM_MOVE) {return;}

	for(auto IA : ArrayIA)
	{
		const FVector halfSize = FVector(DataAssetIA->RandomMoveDistanceFromPlayer, DataAssetIA->RandomMoveDistanceFromPlayer,Player->GetActorLocation().Z);
		const FVector Destination = UKismetMathLibrary::RandomPointInBoundingBox(Player->GetActorLocation(), halfSize);
		IA->Move(Destination, false, DataAssetIA->BaseAcceptanceRadius);
	}
}

void AAIManager::UpdateState(const EIAState& State)
{
	IAState = State;

	if(State == EIAState::RANDOM_MOVE) {return;}
	
	NumberIAToSucceed = ArrayIA.Num();

	const FVector Destination =(Player->GetActorForwardVector() * DataAssetIA->TransfoDistanceFromPlayer) + Player->GetActorLocation();
    for(auto IA : ArrayIA)
    {
        IA->Move(Destination, true, DataAssetIA->BaseAcceptanceRadius);
    }
}

void AAIManager::Spear()
{
	Player->Hide();
	PlayerController->UnPossess();
	
	const FVector Location = Player->GetActorLocation();
	const FRotator Rotation = Player->GetActorRotation();
	FActorSpawnParameters SpawnInfo;
	ASpear* Spear = GetWorld()->SpawnActor<ASpear>(SpearBP, Location, Rotation, SpawnInfo);
	
	PlayerController->Possess(Spear);
}

void AAIManager::Shield()
{
	
}

void AAIManager::Ball()
{
	Player->Hide();
	PlayerController->UnPossess();
}
