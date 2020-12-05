// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ticktacktockPlayerController.h"
#include "UObject/UObjectGlobals.h"
#include "GameFramework/GameUserSettings.h"

AticktacktockPlayerController::AticktacktockPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
