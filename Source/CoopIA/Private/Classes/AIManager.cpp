// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/AIManager.h"
#include "Classes/AIControllerBase.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/CharacterBase.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/GameStateBaseCoop.h"
#include "Classes/Spear.h"

#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

//Library
#include "Classes/Ball.h"
#include "Classes/MainCamera.h"
#include "Classes/Shield.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Tool/HexBehaviour.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAIManager::Init(ACharacterBase* Character, AMainCamera* Camera)
{
	Player = Character;
	CurrentActor = Player;
	PlayerController = Cast<APlayerControllerBase>(Player->GetController());
	MainCamera = Camera;
	NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	PlayerLastHexPos = CurrentActor->GetActorLocation();
	//UE_LOGFMT(LogTemp, Log, "{0}", PlayerController->GetControlRotation().ToString());

	//SPEAR
	FActorSpawnParameters SpawnInfoSpear;
	SpawnInfoSpear.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpearActor = GetWorld()->SpawnActor<ASpear>(SpearBP, CurrentActor->GetActorLocation(), FRotator(), SpawnInfoSpear);
	if (SpearActor)
	{
		SpearActor->SetAIManager(this);
	}
	SpearActor->Hide();

	//SHIELD
	FActorSpawnParameters SpawnInfoShield;
	SpawnInfoShield.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ShieldActor = GetWorld()->SpawnActor<AShield>(ShieldBP, CurrentActor->GetActorLocation(), CurrentActor->GetActorRotation(), SpawnInfoShield);
	ShieldActor->SetOwner(Player);
	ShieldActor->SetActorRelativeRotation(FRotator(0, 0, 0));
	ShieldActor->Hide();

	//BALL
	FActorSpawnParameters SpawnInfoBall;
	SpawnInfoBall.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	BallActor = GetWorld()->SpawnActor<ABall>(BallBP, CurrentActor->GetActorLocation(), FRotator(), SpawnInfoBall);
	if (BallActor)
	{
		BallActor->SetAIManager(this);
	}
	BallActor->Hide();

	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIManager::StaticClass(), FoundManagers);
	
	for (const auto manager : FoundManagers)
	{
		AAIManager* CurrentManager = Cast<AAIManager>(manager);
		if(CurrentManager != this)
		{
			OtherManager = CurrentManager;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player INIT"));
}

void AAIManager::AddPlayer(class ACharacterBaseIA* IA)
{
    ArrayIA.Add(IA);
}

void AAIManager::IASucceededTransition()
{
	NumberIAToSucceed--;
	UE_LOG(LogTemp, Warning, TEXT("Number IA to succeed : %d"),NumberIAToSucceed);
	
	//All IA Moved to the destination
	if(NumberIAToSucceed == 0)
	{
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

		bIsInTransition = false;
	}
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AAIManager::IARandomMove, DataAssetIA->RandomMoveTime, true);

	GetWorld()->GetTimerManager().SetTimer(HandleHexRaycast, this, &AAIManager::FindLastHex, 1, true);
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIManager::RemoveAI(ACharacterBaseIA* IA)
{
	ArrayIA.Remove(IA);
}

void AAIManager::IARandomMove()
{
	if(IAState != EIAState::RANDOM_MOVE) {return;}

	for(auto IA : ArrayIA)
	{
		if(CurrentActor)
		{
			const FVector halfSize = FVector(DataAssetIA->RandomMoveDistanceFromPlayer, DataAssetIA->RandomMoveDistanceFromPlayer,CurrentActor->GetActorLocation().Z);
			const FVector Destination = UKismetMathLibrary::RandomPointInBoundingBox(CurrentActor->GetActorLocation(), halfSize);
			IA->Move(Destination, DataAssetIA->BaseAcceptanceRadius);
		}
	}
}

void AAIManager::FindLastHex()
{
	if(Player && CurrentActor)
	{
		FHitResult HitResult;
		FVector Start = CurrentActor->GetActorLocation();
		FVector End = Start - (FVector::UpVector * 200);	
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(CurrentActor);
		
		if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params, FCollisionResponseParams()))
		{
			AHexBehaviour* Hex = Cast<AHexBehaviour>(HitResult.GetActor());
			if(Hex)
			{
				PlayerLastHexPos = Hex->GetRespawnLoc();
			}
		}
	}
}


void AAIManager::UpdateState(const EIAState& State)
{
	if(State == EIAState::DEAD)
	{
		PlayerDied(IAState);
		return;
	}
	
	if(State == EIAState::REVIVE)
	{
		PlayerRevived(IAState);
		return;
	}

	if(ArrayIA.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to transfo but dont have AI"));
		return;
	}
	
	if(State == IAState || bIsInTransition)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to start same STATE"));
		return;
	}
	bool bShouldGoNeutral = IAState != EIAState::RANDOM_MOVE;
	
	bIsInTransition = true;
	PreviousState = IAState;
	IAState = State;
	//Block Player input

	if(bShouldGoNeutral)
	{
		Neutral(PreviousState);
	}

	if(State == EIAState::RANDOM_MOVE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop transition cause RANDOM MOVE"));
		bIsInTransition = false;
		return;
	}

	NumberIAToSucceed = ArrayIA.Num();
    for(auto IA : ArrayIA)
    {	
        IA->MoveToActor(CurrentActor, DataAssetIA->BaseAcceptanceRadius);
    }
}

TArray<ACharacterBaseIA*> AAIManager::SplitAI()
{
	TArray<ACharacterBaseIA*> CharacterIASplited;
	
	int numberIASplited = 0;
	while (numberIASplited < ArrayIA.Num() / 2)
	{
		CharacterIASplited.Add(ArrayIA[numberIASplited]);
		numberIASplited++;
	}

	for (auto IA_Splited : CharacterIASplited)
	{
		ArrayIA.Remove(IA_Splited);
	}

	return CharacterIASplited;
}

FVector AAIManager::FindLastPos()
{
	return PlayerLastHexPos;
}

void AAIManager::PlayerDied(EIAState State)
{
	IAState = EIAState::DEAD;
	for(auto IA : ArrayIA)
	{
		IA->Destroy();
	}
	ArrayIA.Empty();
	MainCamera->SetPlayer(nullptr, ManagerIndex);

	//Wait time
	UE_LOG(LogTemp, Warning, TEXT("Enter Dead"));
	HidePrevious(State);
	PlayerController->UnPossess();
	PlayerController->SetControlRotation(FRotator());

	Player->SetActorLocation(PlayerLastHexPos, false, nullptr, ETeleportType::TeleportPhysics);
	Player->SetActorRelativeRotation(Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	PlayerController->Possess(Player);
	Player->Died();
	CurrentActor = Player;
	MainCamera->SetPlayer(Player, ManagerIndex);
}

void AAIManager::PlayerRevived(EIAState State)
{
	IAState = EIAState::REVIVE;
	//Wait time
	UE_LOG(LogTemp, Warning, TEXT("Enter Revive"));
	HidePrevious(State);
	PlayerController->UnPossess();
	PlayerController->SetControlRotation(FRotator());

	//TODO::Puzzle or Other Map
	AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState && GameState->GetZoneType() == EZoneType::Puzzle)
	{
		Player->SetActorLocation(GameState->GetRespawnLoc(), false, nullptr, ETeleportType::TeleportPhysics);
	}	
	Player->SetActorRelativeRotation(Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	PlayerController->Possess(Player);
	Player->Revive();

	//Get AI
	ArrayIA = OtherManager->SplitAI();
	InitIA();
	CurrentActor = Player;
	MainCamera->SetPlayer(Player, ManagerIndex);

	//Go to random move
	UpdateState(EIAState::RANDOM_MOVE);
}

void AAIManager::InitIA()
{
	for (auto IA : ArrayIA)
	{
		IA->Init(ManagerIndex);
	}
}

void AAIManager::Spear(EIAState State)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter spear"));
	HidePrevious(State);
	PlayerController->UnPossess();
	PlayerController->SetControlRotation(FRotator());

	FTransform const transform = GetTransfoPos(State);

	if(!SpearActor)
	{
		UE_LOGFMT(LogTemp, Log, "NO SPEAR");
		return;
	}

	SpearActor->SetActorLocation(transform.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	//SpearActor->SetActorRelativeRotation(transform.GetRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	SpearActor->Show();
	
	CurrentActor = SpearActor;
	MainCamera->SetPlayer(SpearActor, ManagerIndex);
	PlayerController->Possess(SpearActor);
}

void AAIManager::Shield(EIAState State)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter shield"));
	if(!ShieldActor)
	{
		UE_LOGFMT(LogTemp, Log, "NO SHIELD");
		return;
	}

	ShieldActor->Show();

	Player->SetupShield(ShieldActor);
}

void AAIManager::Ball(EIAState State)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter Ball"));
	HidePrevious(State);
	PlayerController->UnPossess();
	PlayerController->SetControlRotation(FRotator());
	
	FTransform const transform = GetTransfoPos(State);
	UE_LOG(LogTemp, Warning, TEXT("Previous State Loc %s"), *transform.ToString());
	if(!BallActor)
	{
		UE_LOGFMT(LogTemp, Log, "NO BALL");
		return;
	}

	BallActor->SetActorLocation(transform.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	BallActor->SetActorRelativeRotation(transform.GetRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	BallActor->Show();
	
	CurrentActor = BallActor;
	MainCamera->SetPlayer(BallActor, ManagerIndex);
	PlayerController->Possess(BallActor);
}

void AAIManager::Neutral(EIAState State)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter neutral"));
	HidePrevious(State);
	PlayerController->UnPossess();
	PlayerController->SetControlRotation(FRotator());
	FVector Destination = GetLastPos(State);

	Player->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
	Player->SetActorRelativeRotation(Player->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
	Player->Show();

	CurrentActor = Player;
	MainCamera->SetPlayer(Player, ManagerIndex);
	PlayerController->Possess(Player);

	TeleportIA();
}

void AAIManager::TeleportIA()
{
	const FVector PlayerLoc = CurrentActor->GetActorLocation();
	const float DestinationZ = PlayerLoc.Z;

	for(auto IA : ArrayIA)
	{
		FNavLocation NavLoc;
		FRotator Rotation = IA->GetActorRotation();
		bool bFindDestination = NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 300.f, NavLoc);
		if(bFindDestination)
		{
			FVector TargetLoc = NavLoc.Location;
			IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
			//IA->TeleportTo(FVector(TargetLoc.X, TargetLoc.Y, TargetLoc.Z + DestinationZ), Rotation);
			IA->Show();
		}
		else
		{
			TeleportIAFailed(IA, PlayerLoc, DestinationZ);
		}
	}
}

void AAIManager::TeleportIAFailed(ACharacterBaseIA* IA, FVector PlayerLoc, float DestinationZ)
{
	FNavLocation NavLoc;
	FRotator Rotation = IA->GetActorRotation();
	bool bFindDestination = NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 200.f, NavLoc);
	if(bFindDestination)
	{
		FVector TargetLoc = NavLoc.Location;
		IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
		//IA->TeleportTo(FVector(TargetLoc.X, TargetLoc.Y, TargetLoc.Z + DestinationZ), Rotation);
		IA->Show();
	}
	else
	{
		TeleportIAFailed(IA, PlayerLoc, DestinationZ);
	}
}

void AAIManager::HidePrevious(EIAState State)
{
	switch(State)
	{
		case EIAState::BALL:
			if(Player)
				Player->Hide();
			if(BallActor)
				BallActor->Hide();
			UE_LOG(LogTemp, Warning, TEXT("Hide Ball"));
			break;
		case EIAState::SPEAR:
			if(Player)
				Player->Hide();
			if(SpearActor)
				SpearActor->Hide();
			UE_LOG(LogTemp, Warning, TEXT("Hide Spear"));
			break;
		case EIAState::SHIELD:
			if(Player)
			{
				Player->Hide();
				Player->DeactivateShield();
			}
			UE_LOG(LogTemp, Warning, TEXT("Hide Player + Shield"));
			break;
		case EIAState::RANDOM_MOVE:
			if(Player)
				Player->Hide();
			UE_LOG(LogTemp, Warning, TEXT("Hide Player"));
			break;
	}
}

FTransform AAIManager::GetTransfoPos(EIAState State)
{
	FTransform transform = FTransform();
	switch(State)
	{
		case EIAState::BALL:
			UE_LOG(LogTemp, Warning, TEXT("Get Transfo Ball"));
			break;
		case EIAState::SHIELD:
			UE_LOG(LogTemp, Warning, TEXT("Get Transfo Player"));
			transform.SetLocation(Player->GetActorLocation());
			break;
		case EIAState::SPEAR:
			UE_LOG(LogTemp, Warning, TEXT("Get Transfo Spear"));
			transform.SetLocation(SpearActor->GetActorLocation());
			break;
		case EIAState::RANDOM_MOVE:
			UE_LOG(LogTemp, Warning, TEXT("Get Transfo Player"));
			transform.SetLocation(Player->GetActorLocation());
			break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Teleport to : %s"), *transform.GetLocation().ToString());
	return transform;
}

FVector AAIManager::GetLastPos(EIAState State)
{
	FVector Pos = FVector();
	switch(State)
	{
        case EIAState::BALL:
			Pos = BallActor->GetActorLocation();
			break;
        case EIAState::SHIELD:
			Pos = Player->GetActorLocation();
			break;
        case EIAState::SPEAR:
			Pos = SpearActor->GetActorLocation();
			break;
        case EIAState::RANDOM_MOVE:
			Pos = Player->GetActorLocation();
			break;
		case EIAState::REVIVE:
			Pos = Player->GetActorLocation();
			break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Teleport to : %s"), *Pos.ToString());
	return Pos;
}
