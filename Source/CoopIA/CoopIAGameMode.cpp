// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopIAGameMode.h"
#include "CoopIACharacter.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "Classes/CharacterBase.h"

ACoopIAGameMode::ACoopIAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACoopIAGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	const UWorld* World = GetWorld();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);
	
	int arrayIndex = -1;

	FActorSpawnParameters Params;
	
	for (const auto playerStart : FoundActors)
	{
		arrayIndex++;
		UGameplayStatics::CreatePlayer(World, arrayIndex);

		const FVector Location = playerStart->GetActorLocation();
		const FRotator Rotation = playerStart->GetActorRotation();
		if(arrayIndex>0)
		{
			ACharacterBase* spawnedActor = GetWorld()->SpawnActor<ACharacterBase>(PlayerToSpawn, Location, Rotation);
			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
			playerController->Possess(Cast<APawn>(spawnedActor));
		}
		else
		{
			ACharacterBase* spawnedActor = GetWorld()->SpawnActor<ACharacterBase>(PlayerToSpawn, Location, Rotation);
			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->Possess(Cast<APawn>(spawnedActor));
		}
	}
}
