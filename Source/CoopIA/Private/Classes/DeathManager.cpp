// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/DeathManager.h"
#include "GameplayTagAssetInterface.h"
#include "Classes/AIManager.h"
#include "Components/BoxComponent.h"

// Sets default values
ADeathManager::ADeathManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));
	DeathZone->SetupAttachment(RootComponent);

	OnPlayerGlobalStateChangedDelegate.AddUObject(this, &ThisClass::OnPlayerGlobalStateChanged);

}

void ADeathManager::Init(TArray<AAIManager*>& ArrayAIManager)
{
	AIManager0 = ArrayAIManager[0];
	AIManager1 = ArrayAIManager[1];
}

// Called when the game starts or when spawned
void ADeathManager::BeginPlay()
{
	Super::BeginPlay();

	DeathZone->OnComponentBeginOverlap.AddDynamic(this, &ADeathManager::OnBoxBeginOverlap);
}

// Called every frame
void ADeathManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathManager::OnPlayerGlobalStateChanged(int32 PlayerIndex, EPlayerGlobalState NewPlayerState)
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate called"));
}

void ADeathManager::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnPlayerGlobalStateChangedDelegate.Broadcast(0, EPlayerGlobalState::Alive);
	
	if(OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(OtherActor);
		FGameplayTagContainer OtherActorTag;
		Interface->GetOwnedGameplayTags(OtherActorTag);
		if(OtherActorTag.HasTag(PlayerTag))
		{
			//Revive char, loose IA, update State To Dead
		}
		else if(OtherActorTag.HasTag(AITag))
		{
		
		}
	}
}

void ADeathManager::RevivePlayer()
{
	//Check players State, if both dead GAMEOVER

	//Else
	//Loose IA

	//Revive
	
	
}

