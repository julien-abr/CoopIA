// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopIAGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"
#include "Classes/AIManager.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/GameStateBaseCoop.h"

ACoopIAGameMode::ACoopIAGameMode(){}


void ACoopIAGameMode::BeginPlay()
{
	Super::BeginPlay();
	const UWorld* World = GetWorld();
	
	AMainCamera* MainCamera = Cast<AMainCamera>(UGameplayStatics::GetActorOfClass(World, AMainCamera::StaticClass()));

	AGameStateBaseCoop* GameStateActor = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(GetWorld()));

	/*TArray<AActor*> FoundManagers;
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
	}*/

	APlayerControllerBase* PlayerControllerBase0 = nullptr;
	APlayerControllerBase* PlayerControllerBase1 = nullptr;
	
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
		if(ControllerBase && arrayIndex == 0)
		{
			PlayerControllerBase0 = ControllerBase;
		}
		else
		{
			PlayerControllerBase1 = ControllerBase;
		}
		ControllerBase->Init(MainCamera, arrayIndex);
		playerController->Possess(Cast<APawn>(spawnedActor));

		MainCamera->SetPlayer(spawnedActor, arrayIndex);
		//AAIManager*& Manager = (arrayIndex == 0) ? ManagerPlayer0 : ManagerPlayer1;
		//spawnedActor->Init(Manager);
		//Manager->Init(ControllerBase);
		
		arrayIndex++;
	}
	
	/*TArray<AAIManager*> ArrayAIManager;
	ArrayAIManager.Add(ManagerPlayer0);
	ArrayAIManager.Add(ManagerPlayer1);*/
	
	TArray<APlayerControllerBase*> ArrayPlayerControllerBase;
	ArrayPlayerControllerBase.Add(PlayerControllerBase0);
	ArrayPlayerControllerBase.Add(PlayerControllerBase1);

	for (const auto PlayerController : ArrayPlayerControllerBase)
	{
		PlayerController->LateInit();
	}
	
	if(GameStateActor)
	{
		GameStateActor->Init(ArrayPlayerControllerBase);
	}
}
