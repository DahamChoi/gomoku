// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAsyncTask.h"
#include "Node.h"
#include "TickTakTokGameStateBase.h"
#include "Engine/World.h"
#include "GameAPI.h"

AIAsyncTask::AIAsyncTask(int * action,UWorld * world)
{
	this->ai_action = action;
	this->world = world;
}

void AIAsyncTask::DoWork()
{
	ATickTakTokGameStateBase* gameState =
		world->GetGameState<ATickTakTokGameStateBase>();

	Node* root_node = new Node(new State(gameState->state->mPieces, gameState->state->mEnemyPieces));
	root_node->expand();

	for (int i = 0; i < 100; i++) {
		root_node->evaluate();
	}

	TArray<int32>* smart_legal_actions = gameState->state->smartLegalActions();
	
	int maxValue = 0;
	int maxIndex = 0;
	for (int i = 0; i < root_node->child_nodes.Num(); i++) {
		if (root_node->child_nodes[i]->n > maxValue) {
			maxValue = root_node->child_nodes[i]->n;
			maxIndex = i;
		}
	}

	*this->ai_action = (*smart_legal_actions)[maxIndex];

	/*
	TArray<int32> winPositionArray;
	for (int i = 0; i < root_node->child_nodes.Num(); i++) {
		State* current_state = new State(root_node->state->mPieces, root_node->state->mEnemyPieces);
		float value = root_node->child_nodes[i]->playout(current_state);

		if (FMath::IsNearlyEqual(value, 1.f)) {
			winPositionArray.Add(i);
		}

		delete current_state;
	}

	TArray<int32>* smart_legal_actions = gameState->state->smartLegalActions();

	if (winPositionArray.Num() != 0) {
		*this->ai_action = (*smart_legal_actions)[
			winPositionArray[FMath::RandRange(0, winPositionArray.Num() - 1)]];
	}
	else {
		*this->ai_action = (*smart_legal_actions)[
			FMath::RandRange(0, smart_legal_actions->Num() - 1)];
	}
	*/

	delete root_node;
	delete smart_legal_actions;
}