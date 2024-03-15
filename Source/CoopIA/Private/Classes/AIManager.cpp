// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/AIManager.h"
#include "Classes/Data/EIAState.h"
#include "Classes/AIControllerBase.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/CharacterBase.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/Spear.h"

#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

//Library
#include "Classes/Ball.h"
#include "Classes/MainCamera.h"
#include "Classes/Shield.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	IAState = EIAState::RANDOM_MOVE;
}

void AAIManager::Init(ACharacterBase* Character, AMainCamera* Camera)
{
	Player = Character;
	CurrentActor = Player;
	PlayerController = Cast<APlayerControllerBase>(Player->GetController());
	MainCamera = Camera;
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
		bIsInTransition = false;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
		switch(IAState)
		{
			case EIAState::BALL:
				Ball(PreviousState);
				break;
			case EIAState::SPEAR:
				Spear(PreviousState);
				break;
			case EIAState::SHIELD:
				Shield(PreviousState);
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
		const FVector halfSize = FVector(DataAssetIA->RandomMoveDistanceFromPlayer, DataAssetIA->RandomMoveDistanceFromPlayer,CurrentActor->GetActorLocation().Z);
		const FVector Destination = UKismetMathLibrary::RandomPointInBoundingBox(CurrentActor->GetActorLocation(), halfSize);
		IA->Move(Destination, false, DataAssetIA->BaseAcceptanceRadius);
	}
}

void AAIManager::UpdateState(const EIAState& State)
{
	if(State == EIAState::SHIELD && IAState != EIAState::RANDOM_MOVE){return;}	//Cant use shield if not in neutral form
		
	bIsInTransition = true;
	PreviousState = IAState;
	IAState = State;
	
	for(auto IA : ArrayIA)
	{
		IA->Show();
	}

	if(State == EIAState::RANDOM_MOVE)
	{
		bIsInTransition = false;
		Neutral(PreviousState);
		return;
	}
	
	NumberIAToSucceed = ArrayIA.Num();

	const FVector Destination =(CurrentActor->GetActorForwardVector() * DataAssetIA->TransfoDistanceFromPlayer) + CurrentActor->GetActorLocation();
    for(auto IA : ArrayIA)
    {
        IA->Move(Destination, true, DataAssetIA->BaseAcceptanceRadius);
    }
}

void AAIManager::Spear(EIAState State)
{
	if(bIsInTransition) {return;}
	UE_LOG(LogTemp, Warning, TEXT("Enter spear"));
	HidePrevious(State);
	PlayerController->UnPossess();

	FTransform const transform = GetTransfoPos(State);
	if(!SpearActor)
	{
		FActorSpawnParameters SpawnInfo;
		SpearActor = GetWorld()->SpawnActor<ASpear>(SpearBP, transform.GetLocation(), transform.GetRotation().Rotator(), SpawnInfo);
		SpearActor->SetAIManager(this);
	}
	else
	{
		SpearActor->TeleportTo(transform.GetLocation(), transform.GetRotation().Rotator(), false, true);
		SpearActor->Show();
		UE_LOG(LogTemp, Warning, TEXT("Pos : %s"), *SpearActor->GetActorLocation().ToString());
	}
	
	CurrentActor = SpearActor;
	MainCamera->SetPlayer(SpearActor, ManagerIndex);
	PlayerController->Possess(SpearActor);
}

void AAIManager::Shield(EIAState State)
{
	if(bIsInTransition) {return;}
	UE_LOG(LogTemp, Warning, TEXT("Enter shield"));
	if(!ShieldActor)
	{
		FActorSpawnParameters SpawnInfo;
		ShieldActor = GetWorld()->SpawnActor<AShield>(ShieldBP, CurrentActor->GetActorLocation(), CurrentActor->GetActorRotation(), SpawnInfo);
		ShieldActor->AttachToComponent(Player->GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		ShieldActor->Show();
	}
}

void AAIManager::Ball(EIAState State)
{
	if(bIsInTransition) {return;}
	UE_LOG(LogTemp, Warning, TEXT("Enter Ball"));
	HidePrevious(State);
	PlayerController->UnPossess();

	FTransform const transform = GetTransfoPos(State);
	if(!BallActor)
	{
		FActorSpawnParameters SpawnInfo;
		BallActor = GetWorld()->SpawnActor<ABall>(BallBP, transform.GetLocation(), transform.GetRotation().Rotator(), SpawnInfo);
		BallActor->SetAIManager(this);
	}
	else
	{
		BallActor->TeleportTo(transform.GetLocation(), transform.GetRotation().Rotator(), false, true);
		BallActor->Show();
	}
	
	CurrentActor = BallActor;
	MainCamera->SetPlayer(BallActor, ManagerIndex);
	PlayerController->Possess(BallActor);
}

void AAIManager::Neutral(EIAState State)
{
	if(bIsInTransition) {return;}
	UE_LOG(LogTemp, Warning, TEXT("Enter neutral"));
	HidePrevious(State);
	PlayerController->UnPossess();

	FTransform const transform = GetTransfoPos(State);

	Player->TeleportTo(transform.GetLocation(), transform.GetRotation().Rotator(), false, true);
	Player->Show();

	CurrentActor = Player;
	MainCamera->SetPlayer(Player, ManagerIndex);
	PlayerController->Possess(Player);
}

void AAIManager::HidePrevious(EIAState State)
{
	switch(State)
	{
		case EIAState::BALL:
			BallActor->Hide();
			break;
		case EIAState::SPEAR:
			SpearActor->Hide();
			break;
		case EIAState::SHIELD:
			Player->Hide();
			ShieldActor->Hide();
			break;
		case EIAState::RANDOM_MOVE:
			Player->Hide();
			break;
	}
}

FTransform AAIManager::GetTransfoPos(EIAState State)
{
	FTransform transform;
	switch(State)
	{
		case EIAState::BALL:
			transform.SetLocation(BallActor->GetActorLocation());
			transform.SetRotation(BallActor->GetActorRotation().Quaternion());
			return transform;
		case EIAState::SHIELD:
			transform.SetLocation(Player->GetActorLocation());
			transform.SetRotation(Player->GetActorRotation().Quaternion());
			return transform;
		case EIAState::SPEAR:
			transform.SetLocation(SpearActor->GetActorLocation());
			transform.SetRotation(SpearActor->GetActorRotation().Quaternion());
			return transform;
		case EIAState::RANDOM_MOVE:
			transform.SetLocation(Player->GetActorLocation());
			transform.SetRotation(Player->GetActorRotation().Quaternion());
			return transform;
	}
	return transform;
}
