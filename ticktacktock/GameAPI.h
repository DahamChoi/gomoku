// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"

/**
 * 
 */
class TICKTACKTOCK_API GameAPI
{
public:
	GameAPI();
	~GameAPI();

public:
	static FVector ActionToStoneVector(int action);
	static int StoneVectorToAction(FVector vector);
	static bool nextAction(int action, UWorld* world);
	static void setTextRender(UWorld* world,FString text, FColor color, float time = -1.f);
};
