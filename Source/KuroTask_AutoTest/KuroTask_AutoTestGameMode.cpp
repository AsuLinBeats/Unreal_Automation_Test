// Copyright Epic Games, Inc. All Rights Reserved.

#include "KuroTask_AutoTestGameMode.h"
#include "KuroTask_AutoTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKuroTask_AutoTestGameMode::AKuroTask_AutoTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
