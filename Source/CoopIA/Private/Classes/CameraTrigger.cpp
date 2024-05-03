// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CameraTrigger.h"
#include "Classes/MainCamera.h"

//Library
#include <Data/Interface/PlayerInterface.h>

#include "Classes/GameStateBaseCoop.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraTrigger::ACameraTrigger()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACameraTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACameraTrigger::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACameraTrigger::OnBoxEndOverlap);
	MainCamera = Cast<AMainCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCamera::StaticClass()));
}

void ACameraTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(UKismetSystemLibrary::DoesImplementInterface(OtherActor, UPlayerInterface::StaticClass()) && bCanOverlap)
    {
    	const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(OtherActor);
    	if(Index == 0)
    	{
    		Player0 = OtherActor;
    	}
    	else
    	{
    		Player1 = OtherActor;
    	}
    }

	if(Player0 && Player1)
	{
		switch(TriggerCamera)
		{
		case ECameraState::FIXED:
			MainCamera->SetFixedPosition(ActorFixedPos->GetActorTransform());
			break;
		case ECameraState::FOLLOW:
			MainCamera->SetSpline(FollowSpline);
			break;
		}
		
		AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(GetWorld()));
		GameState->SetZoneInfo(ZoneType, LevelSide);

		WallActor->SetActorEnableCollision(true);
		WallActor->SetActorHiddenInGame(false);
		bCanOverlap = false;
	}
}

void ACameraTrigger::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(UKismetSystemLibrary::DoesImplementInterface(OtherActor, UPlayerInterface::StaticClass()) && bCanOverlap)
	{
		const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(OtherActor);
		if(Index == 0)
		{
			Player0 = nullptr;
		}
		else
		{
			Player1 = nullptr;
		}
	}
}


