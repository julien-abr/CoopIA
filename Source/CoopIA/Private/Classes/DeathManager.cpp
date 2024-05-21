// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/DeathManager.h"
#include "GameplayTagAssetInterface.h"
#include "Classes/AIManager.h"
#include "Components/BoxComponent.h"
#include "Data/Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeathManager::ADeathManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));
	DeathZone->SetupAttachment(RootComponent);
}

void ADeathManager::RevivePlayer(int32 Index)
{
	OnPlayerGlobalStateChangedDelegate.Broadcast(Index, EPlayerGlobalState::Alive);
}

// Called when the game starts or when spawned
void ADeathManager::BeginPlay()
{
	Super::BeginPlay();

	DeathZone->OnComponentBeginOverlap.AddDynamic(this, &ADeathManager::OnBoxBeginOverlap);
	
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIManager::StaticClass(), FoundManagers);
	for (auto ManagerActor : FoundManagers)
	{	AAIManager* IAManager = Cast<AAIManager>(ManagerActor);
		if(!IAManager) {return;}
		if(IAManager->ManagerIndex == 0)
		{
			Manager0 = IAManager;
		}
		else
		{
			Manager1 = IAManager;
		}
	}
}

void ADeathManager::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{		
	if(OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(OtherActor);
		FGameplayTagContainer OtherActorTag;
		Interface->GetOwnedGameplayTags(OtherActorTag);
		if(OtherActorTag.HasTag(PlayerTag))
		{
			//Revive char, loose IA, update State To Dead
			if(OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
			{
				int32 Index = IPlayerInterface::Execute_GetPlayerIndex(OtherActor);
				UE_LOG(LogTemp, Warning, TEXT("Player hit DeathZone"));
				OnPlayerGlobalStateChangedDelegate.Broadcast(Index, EPlayerGlobalState::Dead);
			}

		}
		else if(OtherActorTag.HasTag(AITag))
		{
			UE_LOG(LogTemp, Warning, TEXT("AI falled in DeathZone"));
		}
	}
}

