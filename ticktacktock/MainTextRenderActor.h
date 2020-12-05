// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "MainTextRenderActor.generated.h"

/**
 * 
 */
UCLASS()
class TICKTACKTOCK_API AMainTextRenderActor : public ATextRenderActor
{
	GENERATED_BODY()

public:
	FTimerHandle timerHandle;
	void AdvanceTimer();

	void setTextRenderTimer(float time);
};
