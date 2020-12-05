// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAPIActor.h"
#include "TickTakTokGameStateBase.h"
#include "GoStone.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "MainTextRenderActor.h"
#include "Components/TextRenderComponent.h"

constexpr auto X_MARGIN = 2.17;
constexpr auto Y_MARGIN = 2.37;

// Sets default values
AGameAPIActor::AGameAPIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameAPIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameAPIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATickTakTokGameStateBase* gameState = GetWorld()->GetGameState<ATickTakTokGameStateBase>();

	if (AITask != nullptr) {
		if (AITask->IsWorkDone()) {
			this->nextAction(this->ai_action);
			if (!gameState->state->isDone()) {
				this->setTextRender(TEXT(""), FColor::FColor(255, 255, 255));
			}

			AITask = nullptr;
		}
	}
}

FVector AGameAPIActor::ActionToStoneVector(int action)
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

int AGameAPIActor::StoneVectorToAction(FVector vector)
{
	int yindex = round((vector.Y + (Y_MARGIN * 8)) / Y_MARGIN) - 1;
	int xindex = round((vector.X + (X_MARGIN * 8)) / X_MARGIN) - 1;
	int action = yindex * 15 + xindex;

	return action;
}

bool AGameAPIActor::nextAction(int action)
{
	ATickTakTokGameStateBase* gameState = GetWorld()->GetGameState<ATickTakTokGameStateBase>();
	TArray<int32>* actions;
	actions = gameState->state->legalActions();

	if (actions->Contains(action))
	{
		if (readyStoneActor != NULL) {
			GetWorld()->DestroyActor(readyStoneActor);
			readyStoneActor = NULL;
		}
		for (int i = 0; i < unvalueStoneArray.Num(); i++) {
			GetWorld()->DestroyActor(unvalueStoneArray[i]);
		}
		unvalueStoneArray.Empty();

		AGoStone* stone = GetWorld()->SpawnActor<AGoStone>(
			this->ActionToStoneVector(action), FRotator::ZeroRotator);
		stone->setType(gameState->state->isFirstPlayer() ? 0 : 1);

		gameState->state = gameState->state->next(action);

		if (gameState->state->isDone())
		{
			this->setTextRender(
				gameState->state->isFirstPlayer() && gameState->state->isLose() ?
				TEXT("WHITE WIN!") : TEXT("BLACK WIN!"), FColor::FColor(255, 255, 255));
		}
		else
		{
			if (gameState->state->isFirstPlayer())
			{
				TArray<int32>* unvalueActions = gameState->state->unvalueActions();
				for (int i = 0; i < unvalueActions->Num(); i++)
				{
					AGoStone* redStone = GetWorld()->SpawnActor<AGoStone>(
						this->ActionToStoneVector((*unvalueActions)[i]), FRotator::ZeroRotator);
					redStone->setType(2);
					unvalueStoneArray.Add(redStone);
				}

				delete unvalueActions;
			}
		}

		delete actions;
		return true;
	}
	else
	{
		delete actions;
		return false;
	}
}

void AGameAPIActor::setTextRender(FString text, FColor color,bool visible, float time)
{
	AMainTextRenderActor* mainTextRenderActor = NULL;
	for (TActorIterator<AMainTextRenderActor> entity(GetWorld()); entity; ++entity)
	{
		mainTextRenderActor = (*entity);
	}

	if (mainTextRenderActor != nullptr) {
		mainTextRenderActor->GetTextRender()->SetVisibility(visible);
		mainTextRenderActor->GetTextRender()->SetTextRenderColor(color);
		mainTextRenderActor->GetTextRender()->SetText(text);

		if (time > 0.f) {
			mainTextRenderActor->setTextRenderTimer(time);
		}
	}
}

void AGameAPIActor::AIPlay()
{
	this->setTextRender(TEXT("AI TURN..."), FColor::FColor(255, 255, 255));

	AITask = new FAsyncTask<AIAsyncTask>(&this->ai_action, GetWorld());
	AITask->StartBackgroundTask();
}

void AGameAPIActor::resetGame()
{
	ATickTakTokGameStateBase* gameState = GetWorld()->GetGameState<ATickTakTokGameStateBase>();
	gameState->state->gameReset();

	AGoStone* goStone = NULL;
	for (TActorIterator<AGoStone> entity(GetWorld()); entity; ++entity)
	{
		GetWorld()->DestroyActor((*entity));
	}

	this->setTextRender(TEXT(""), FColor::FColor(0, 0, 0), false);
}

bool AGameAPIActor::readyStone(int action)
{
	ATickTakTokGameStateBase* gameState = GetWorld()->GetGameState<ATickTakTokGameStateBase>();
	TArray<int32> * legal_actions = gameState->state->legalActions();
	if (legal_actions->Contains(action)) {
		if(readyStoneActor != NULL){
			GetWorld()->DestroyActor(readyStoneActor);
		}

		readyStoneActor = GetWorld()->SpawnActor<AGoStone>(
			this->ActionToStoneVector(action), FRotator::ZeroRotator);
		readyStoneActor->setType(3);

		delete legal_actions;
		return true;
	}
	else {
		delete legal_actions;
		return false;
	}
}

