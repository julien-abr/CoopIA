// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/DeathManager.h"
#include "GameplayTagAssetInterface.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/StateMachine/StateMachineComponent.h"
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
	
	TArray<AActor*> ArrayPlayerControllerBase;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerControllerBase::StaticClass(), ArrayPlayerControllerBase);

	for (auto ActorController : ArrayPlayerControllerBase)
	{
		APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(ActorController);
		if(PlayerControllerBase->GetPlayerIndex() == 0)
		{
			ST_Player0 = PlayerControllerBase->GetStateMachineComponent();
		}
		else
		{
			ST_Player1 =  PlayerControllerBase->GetStateMachineComponent();
		}
	}
}

void ADeathManager::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{		
	if(OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Hit Zone : %s"), *OtherActor->GetName());
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
			ACharacterBaseIA* AI = Cast<ACharacterBaseIA>(OtherActor);
			if (!AI) { return; }
			if (AI->PlayerIndex == 0)
			{
				ST_Player0->RemoveAI(AI);
			}
			else
			{
				ST_Player1->RemoveAI(AI);
			}
		}
	}
}

