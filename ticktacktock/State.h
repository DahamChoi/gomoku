// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoStone.h"

/**
 * 
 */
class TICKTACKTOCK_API State
{
public:
	TArray<int32>* mPieces;
	TArray<int32>* mEnemyPieces;

public:
	State();
	State(TArray<int32>* pieces, TArray<int32>* enemyPieces);
	~State();

public:
	int getPieceCount(TArray<int32>* pieces);

	bool isLose();
	int getCompCount(int x, int y, int dx, int dy,TArray<int32>* pieces);

	bool isDraw();

	bool isDone();

	void next_void(int action);
	State * next(int action);

	TArray<int32>* legalActions();
	TArray<int32>* smartLegalActions();

	TArray<int32>* unvalueActions();
	
	TArray<int32>* checkLineType(TArray<int32>* pieces, TArray<int32>* enemyPieces, int pos, int dx, int dy);
	
	int checkLineTypeSpeical(TArray<int32>* pieces, TArray<int32>* enemyPieces, int pos, int type);
	
	bool checkEmpty(int pos);

	bool equal_array(TArray<int32>* array1, TArray<int32>* array2);

	bool isFirstPlayer();

	void gameReset();
};
