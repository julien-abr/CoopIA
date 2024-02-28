// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopIAGameMode.h"
#include "CoopIACharacter.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "Classes/CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACoopIAGameMode::ACoopIAGameMode(){}

void ACoopIAGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	/*const UWorld* World = GetWorld();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);
	
	int arrayIndex = 0;

	FActorSpawnParameters Params;
	
	for (const auto playerStart : FoundActors)
	{
		APlayerController* Controller = UGameplayStatics::CreatePlayer(World, arrayIndex);

		if(!Controller)
		{
			UE_LOG(LogTemp,Warning,TEXT("Controller NULL"));
		}

		const FVector Location = playerStart->GetActorLocation();
		const FRotator Rotation = playerStart->GetActorRotation();
		if(arrayIndex>0)
		{
			//Add Input Mapping Context
			if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
			
			ACharacterBase* spawnedActor = GetWorld()->SpawnActor<ACharacterBase>(PlayerToSpawn, Location, Rotation);
			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
			playerController->Possess(Cast<APawn>(spawnedActor));
		}
		else
		{
			if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
			
			ACharacterBase* spawnedActor = GetWorld()->SpawnActor<ACharacterBase>(PlayerToSpawn, Location, Rotation);
			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerController->Possess(Cast<APawn>(spawnedActor));
		}

		arrayIndex++;
	}*/
}
