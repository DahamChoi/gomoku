// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAPI.h"
#include "TickTakTokGameStateBase.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "TimerManager.h"
#include "GoStone.h"
#include "ticktacktockPawn.h"
#include "MainTextRenderActor.h"

constexpr auto X_MARGIN = 2.17;
constexpr auto Y_MARGIN = 2.37;

GameAPI::GameAPI(){}
GameAPI::~GameAPI(){}

FVector GameAPI::ActionToStoneVector(int action)
{
	// 0 stone actor : x - -15.19, y - -16.59, z - 32.7
	int x = action % 15;
	int y = action / 15;

	FVector vector;
	vector.Z = 32.7f;
	vector.X = -15.19f + (X_MARGIN * x);
	vector.Y = -16.59f + (Y_MARGIN * y);

	return vector;
}

int GameAPI::StoneVectorToAction(FVector vector)
{
	int yindex = round((vector.Y + (Y_MARGIN * 8)) / Y_MARGIN) - 1;
	int xindex = round((vector.X + (X_MARGIN * 8)) / X_MARGIN) - 1;
	int action = yindex * 15 + xindex;

	return action;
}

// should be call gameplay task
// spawn stone and game state change
bool GameAPI::nextAction(int action, UWorld* world)
{
	ATickTakTokGameStateBase* gameState = world->GetGameState<ATickTakTokGameStateBase>();
	TArray<int32>* actions;
	actions = gameState->state->legalActions();

	if (actions->Contains(action))
	{
		AGoStone* stone = world->SpawnActor<AGoStone>(GameAPI::ActionToStoneVector(action), FRotator::ZeroRotator);
		stone->setType(gameState->state->isFirstPlayer() ? 0 : 1);

		gameState->state = gameState->state->next(action);

		if (gameState->state->isDone())
		{
			ATextRenderActor* textrender;
			for (TActorIterator<ATextRenderActor> entity(world); entity; ++entity)
			{
				textrender = (*entity);
			}

			GameAPI::setTextRender(world,
				gameState->state->isFirstPlayer() && gameState->state->isLose() ?
				TEXT("WHITE WIN!") : TEXT("BLACK WIN!"), FColor::FColor(255, 255, 255),10.f);
		}

		return true;
	}
	else
	{
		GameAPI::setTextRender(world,TEXT("FORBIDDEN<br>PLACE!"), FColor::FColor(128, 0, 0), 1.f);

		return false;
	}
}

void GameAPI::setTextRender(UWorld* world, FString text, FColor color, float time)
{
	AMainTextRenderActor* mainTextRenderActor = NULL;
	for (TActorIterator<AMainTextRenderActor> entity(world); entity; ++entity)
	{
		mainTextRenderActor = (*entity);
	}

	if (mainTextRenderActor != nullptr) {
		mainTextRenderActor->GetTextRender()->SetVisibility(true);
		mainTextRenderActor->GetTextRender()->SetTextRenderColor(color);
		mainTextRenderActor->GetTextRender()->SetText(text);

		if (time > 0.f) {
			mainTextRenderActor->setTextRenderTimer(time);
		}
	}
}