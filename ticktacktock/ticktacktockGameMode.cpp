// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ticktacktockGameMode.h"
#include "ticktacktockPlayerController.h"
#include "ticktacktockPawn.h"
#include "TickTakTokGameStateBase.h"

AticktacktockGameMode::AticktacktockGameMode()
{
	// no pawn by default
	DefaultPawnClass = AticktacktockPawn::StaticClass();

	// use our own player controller class
	PlayerControllerClass = AticktacktockPlayerController::StaticClass();
	

	GameStateClass = ATickTakTokGameStateBase::StaticClass();
}
