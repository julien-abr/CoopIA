// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopIAGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/AIManager.h"
#include "Classes/PlayerControllerBase.h"

ACoopIAGameMode::ACoopIAGameMode(){}


void ACoopIAGameMode::BeginPlay()
{
	Super::BeginPlay();
	const UWorld* World = GetWorld();
	
	AMainCamera* MainCamera = Cast<AMainCamera>(UGameplayStatics::GetActorOfClass(World, AMainCamera::StaticClass()));

	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(World, AAIManager::StaticClass(), FoundManagers);

	AAIManager* ManagerPlayer0;
	AAIManager* ManagerPlayer1;
	for (const auto manager : FoundManagers)
	{
		AAIManager* CurrentManager = Cast<AAIManager>(manager);
		if(CurrentManager->ManagerIndex == 0)
		{
			ManagerPlayer0 = CurrentManager;
		}
		else
		{
			ManagerPlayer1 = CurrentManager;
		}
	}
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);

	FActorSpawnParameters Params;
	
	for (const auto playerStart : FoundActors)
	{
		APlayerController* Controller = UGameplayStatics::CreatePlayer(World, arrayIndex);

		const FVector Location = playerStart->GetActorLocation();
		const FRotator Rotation = playerStart->GetActorRotation();
		
		ACharacterBase* spawnedActor = GetWorld()->SpawnActor<ACharacterBase>(PlayerToSpawn, Location, Rotation);
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), arrayIndex);
		APlayerControllerBase* ControllerBase = Cast<APlayerControllerBase>(playerController);
		ControllerBase->Init(MainCamera);
		playerController->Possess(Cast<APawn>(spawnedActor));

		MainCamera->SetPlayer(spawnedActor, arrayIndex);
		AAIManager*& Manager = (arrayIndex == 0) ? ManagerPlayer0 : ManagerPlayer1;
		spawnedActor->Init(Manager);
		Manager->Init(spawnedActor, MainCamera);
		
		arrayIndex++;
	}
}
