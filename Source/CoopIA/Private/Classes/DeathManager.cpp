// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/DeathManager.h"
#include "Classes/AIManager.h"
#include "Components/BoxComponent.h"

// Sets default values
ADeathManager::ADeathManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));
	DeathZone->SetupAttachment(RootComponent);

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

void ADeathManager::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		
	}
}

