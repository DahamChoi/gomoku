// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTextRenderActor.h"
#include "TimerManager.h"
#include "Components/TextRenderComponent.h"

void AMainTextRenderActor::AdvanceTimer()
{
	this->GetTextRender()->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(timerHandle);
}

void AMainTextRenderActor::setTextRenderTimer(float time)
{
	GetWorldTimerManager().SetTimer(
		timerHandle, this, &AMainTextRenderActor::AdvanceTimer, time, true);
}
