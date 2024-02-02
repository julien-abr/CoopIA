// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopIAGameMode.h"
#include "CoopIACharacter.h"
#include "UObject/ConstructorHelpers.h"

ACoopIAGameMode::ACoopIAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
