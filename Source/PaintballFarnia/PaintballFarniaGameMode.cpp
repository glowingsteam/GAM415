// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "PaintballFarniaGameMode.h"
#include "PaintballFarniaHUD.h"
#include "PaintballFarniaCharacter.h"
#include "UObject/ConstructorHelpers.h"

APaintballFarniaGameMode::APaintballFarniaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APaintballFarniaHUD::StaticClass();
}
