// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "GameFramework/GameStateBase.h"
#include "TickTakTokGameStateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TICKTACKTOCK_API ATickTakTokGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATickTakTokGameStateBase();
	~ATickTakTokGameStateBase();

public:
	State* state;

public:
	UFUNCTION(BlueprintCallable)
	bool isGameDone();

	UFUNCTION(BlueprintCallable)
	bool isFirstPlayer();

public:
	UPROPERTY(BlueprintReadWrite)
	bool isPawnFirstPlayer;

	UPROPERTY(BlueprintReadWrite)
	bool isLevelGameDone;
};
