// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"
#include "UObject/UObjectIterator.h"

State::State()
{
	mPieces = new TArray<int32>();
	mEnemyPieces = new TArray<int32>();

	mPieces->Init(0, 225);
	mEnemyPieces->Init(0, 225);
}

State::State(TArray<int32>* pieces, TArray<int32>* enemyPieces)
{
	mPieces = new TArray<int32>(*pieces);
	mEnemyPieces = new TArray<int32>(*enemyPieces);
}

State::~State()
{
	delete mPieces;
	delete mEnemyPieces;
}

int State::getPieceCount(TArray<int32>* pieces)
{
	int count = 0;
	for (int i = 0; i < pieces->Num(); i++) 
	{
		if ((*pieces)[i] == 1)
		{
			++count;
		}
	}

	return count;
}

bool State::isLose()
{
	for (int i = 0; i < 225; i++)
	{
		int ix = i % 15;
		int iy = i / 15;
		static int dist_list[4][4] = {
			{1,0,-1,0},{0,1,0,-1},{1,1,-1,-1},{1,-1,-1,1}
		};

		for (int j = 0; j < 4; j++)
		{
			int dx1 = dist_list[j][0];
			int dy1 = dist_list[j][1];
			int dx2 = dist_list[j][2];
			int dy2 = dist_list[j][3];
			int pieceCount = getCompCount(ix, iy, dx1, dy1,this->mEnemyPieces) + getCompCount(ix, iy, dx2, dy2,this->mEnemyPieces) - 1;
			if (this->isFirstPlayer() ? pieceCount >= 5 : pieceCount == 5)
			{
				return true;
			}
		}
	}
	return false;
}

int State::getCompCount(int x, int y, int dx, int dy,TArray<int32> * pieces)
{
	int count = 0;
	while (true)
	{
		if (y < 0 || y > 14 || x < 0 || x > 14 || (*pieces)[y * 15 + x] == 0)
		{
			break;
		}
		x = x + dx;
		y = y + dy;
		++count;
	}

	return count;
}

bool State::isDraw()
{
	return (this->getPieceCount(mPieces) + this->getPieceCount(mEnemyPieces) == 225);
}

bool State::isDone()
{
	return (this->isLose() || this->isDraw());
}

void State::next_void(int action)
{
	(*mPieces)[action] = 1;
	TArray<int32>* temp = mPieces;
	mPieces = mEnemyPieces;
	mEnemyPieces = temp;
}

State * State::next(int action)
{
	TArray<int32>* pieces = new TArray<int32>(*mPieces);
	(*pieces)[action] = 1;

	State* state = new State(mEnemyPieces, pieces);
	delete pieces;

	return state;
}

TArray<int32>* State::legalActions()
{
	TArray<int32>* actions = new TArray<int32>();
	for (int i = 0; i < 225; i++)
	{
		if ((*this->mPieces)[i] == 0 && (*this->mEnemyPieces)[i] == 0)
		{
			if (this->isFirstPlayer())
			{
				TArray<int32>* tempPieces = new TArray<int32>((*this->mPieces));
				(*tempPieces)[i] = 1;

				if (this->checkLineTypeSpeical(tempPieces,this->mEnemyPieces,i,0) < 2)
				{
					actions->Add(i);
				}

				delete tempPieces;
			}
			else
			{
				actions->Add(i);
			}
		}
	}

	return actions;
}

TArray<int32>* State::smartLegalActions()
{
	TArray<int32>* actions = new TArray<int32>();
	TArray<int32>* default_legal_actions = this->legalActions();
	TArray<int32>* remove_object = new TArray<int32>();
	for (int i = 0; i < default_legal_actions->Num(); i++) {
		if (checkEmpty((*default_legal_actions)[i])) {
			remove_object->Add((*default_legal_actions)[i]);
		}
	}

	for (int i = 0; i < remove_object->Num(); i++) {
		default_legal_actions->Remove((*remove_object)[i]);
	}

	delete remove_object;

	int dist_list[4][4] = {
		{1,0,-1,0},
		{0,1,0,-1},
		{1,1,-1,-1},
		{1,-1,-1,1}
	};

	// 내가 5목을 만들 수 있는 상황이면 반드시 승리를 쟁취한다.
	// 상대방이 i 액션을 했을 때 5목을 만든다면 반드시 막는다.
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < 225; i++) {
			if (default_legal_actions->Contains(i)) {
				TArray<int32>* temp_pieces;
				temp_pieces = new TArray<int32>(x == 0 ? *this->mPieces : *this->mEnemyPieces);
				(*temp_pieces)[i] = 1;

				int ix = i % 15;
				int iy = i / 15;

				for (int j = 0; j < 4; j++) {
					int piece_count =
						getCompCount(ix, iy, dist_list[j][0], dist_list[j][1], temp_pieces) 
						+ getCompCount(ix, iy, dist_list[j][2], dist_list[j][3], temp_pieces) - 1;
					if ((x == 0 ? this->isFirstPlayer() : !this->isFirstPlayer()) ?
						piece_count == 5 : piece_count >= 5) {
						actions->RemoveAt(0, actions->Num(), false);
						actions->Add(i);
						delete default_legal_actions;
						return actions;
					}
				}

				delete temp_pieces;
			}
		}
	}

	for (int i = 0; i < 225; i++) {
		if (default_legal_actions->Contains(i)) {
			// 내가 열린 4를 만들 수 있는 상황이라면 반드시 공격한다.
			// 내가 4:3을 만들 수 있는 상황이라면 반드시 공격한다.
			TArray<int32>* temp_pieces = new TArray<int32>(*this->mPieces);
			(*temp_pieces)[i] = 1;

			int open_four_count = checkLineTypeSpeical(temp_pieces, this->mEnemyPieces, i, 2);
			int close_four_count = checkLineTypeSpeical(temp_pieces, this->mEnemyPieces, i, 1);
			int open_three_count = checkLineTypeSpeical(temp_pieces, this->mEnemyPieces, i, 0);

			if (open_four_count >= 1) {
				actions->RemoveAt(0, actions->Num(), false);
				actions->Add(i);
				delete default_legal_actions;
				return actions;
			}
			
			if (close_four_count >= 1 && open_three_count >= 1) {
				actions->RemoveAt(0, actions->Num(), false);
				actions->Add(i);
				delete default_legal_actions;
				return actions;
			}

			delete temp_pieces;
		}
	}

	bool is_have_open_three = false;
	for (int i = 0; i < 225; i++) {
		if (default_legal_actions->Contains(i)) {
			// 열린 3은 막는다.
			int open_three_count = checkLineTypeSpeical(this->mEnemyPieces, this->mPieces, i,0);
			if (open_three_count >= 1) {
				actions->Add(i);
				is_have_open_three = true;
			}

			TArray<int32>* temp_pieces = new TArray<int32>(*this->mEnemyPieces);
			(*temp_pieces)[i] = 1;

			// 내가 선공일때 상대방이 i 지점에 쌍삼을 만들 수 있다면 막는다.
			open_three_count = checkLineTypeSpeical(temp_pieces, this->mPieces, i, 0);
			if (this->isFirstPlayer()) {
				if (open_three_count >= 2) {
					actions->Add(i);
					is_have_open_three = true;
				}
			}

			int close_four_count = checkLineTypeSpeical(temp_pieces, this->mPieces, i, 1);
			int open_four_count = checkLineTypeSpeical(temp_pieces, this->mPieces, i, 2);

			if ((close_four_count >= 1 || open_four_count >= 1 ) && open_three_count >= 1) {
				actions->Add(i);
				is_have_open_three = true;
			}

			delete temp_pieces;
		}
	}

	// 내가 닫힌 4를 만들 수 있는 상황이라면 상대방이 열린3or쌍삼이여도 공격할 수 있다.
	for (int i = 0; i < 225; i++) {
		if (default_legal_actions->Contains(i) && is_have_open_three) {
			if (!this->isFirstPlayer()) {
				TArray<int32>* temp_pieces = new TArray<int32>(*this->mPieces);
				(*temp_pieces)[i] = 1;

				int close_four_count = checkLineTypeSpeical(temp_pieces, this->mEnemyPieces, i, 1);
				if (close_four_count >= 1) {
					actions->Add(i);
				}

				delete temp_pieces;
			}
		}
	}

	// 내가 후공일때 쌍삼을 만들 수 있다면 공격한다.
	for (int i = 0; i < 225; i++) {
		if (default_legal_actions->Contains(i) && actions->Num() == 0) {
			if (!this->isFirstPlayer()) {
				TArray<int32>* temp_pieces = new TArray<int32>(*this->mPieces);
				(*temp_pieces)[i] = 1;

				int open_three_count = checkLineTypeSpeical(temp_pieces, this->mEnemyPieces, i, 0);
				if (open_three_count >= 2) {
					actions->Add(i);
				}

				delete temp_pieces;
			}
		}
	}

	if (actions->Num() == 0) {
		delete actions;
		return default_legal_actions;
	}

	delete default_legal_actions;
	return actions;
}

TArray<int32>* State::unvalueActions()
{
	TArray<int32>* actions = new TArray<int32>();
	for (int i = 0; i < 225; i++)
	{
		if ((*this->mPieces)[i] == 0 && (*this->mEnemyPieces)[i] == 0)
		{
			if (this->isFirstPlayer())
			{
				TArray<int32>* tempPieces = new TArray<int32>((*this->mPieces));
				(*tempPieces)[i] = 1;

				int open_three_count = this->checkLineTypeSpeical(tempPieces, this->mEnemyPieces, i, 0);
				int open_four_count = this->checkLineTypeSpeical(tempPieces, this->mEnemyPieces, i, 2);
				int close_four_count = this->checkLineTypeSpeical(tempPieces, this->mEnemyPieces, i, 1);

				if (open_three_count >= 2 || open_four_count + close_four_count >= 2)
				{
					actions->Add(i);
				}

				delete tempPieces;
			}
		}
	}

	return actions;
}

TArray<int32>* State::checkLineType(TArray<int32>* pieces, TArray<int32>* enemyPieces, int pos, int dx, int dy)
{
	int posX = pos % 15;
	int posY = pos / 15;
	
	int dstPos = 0;

	int emptyCount = 0;
	int startX = 0;
	int startY = 0;

	TArray<int32>* lineType = new TArray<int32>();

	if ((*pieces)[pos] == 0 && (*enemyPieces)[pos] == 0) {
		emptyCount += 1;
	}

	while (true)
	{
		posX = posX + dx;
		posY = posY + dy;
		dstPos = posY * 15 + posX;

		if (posX < 0 && posY < 0)
		{
			startX = 0;
			startY = 0;
			break;
		}
		if (posX < 0)
		{
			startX = 0;
			startY = posY - dy;
			break;
		}
		if (posY < 0)
		{
			startX = posX - dy;
			startY = 0;
			break;
		}
		if (posX > 14 && posY > 14)
		{
			startX = 14;
			startY = 14;
			break;
		}
		if (posX > 14)
		{
			startX = 14;
			startY = posY - dy;
			break;
		}
		if (posY > 14)
		{
			startX = posX - dx;
			startY = 14;
			break;
		}

		if ((*pieces)[dstPos] == 0 && (*enemyPieces)[dstPos] == 0)
		{
			emptyCount++;
			if (emptyCount >= 2)
			{
				startX = posX - dx;
				startY = posY - dy;
				break;
			}
		}
		else if ((*enemyPieces)[dstPos] == 1)
		{
			startX = posX - dx;
			startY = posY - dy;
			break;
		}
		else if ((*mPieces)[dstPos] == 1)
		{
			emptyCount = 0;
		}
	}

	emptyCount = 0;
	dstPos = startY * 15 + startX;
	if (dstPos < 0 || dstPos >= 225)
	{
		return lineType;
	}

	posX = dstPos % 15;
	posY = dstPos / 15;

	while (true)
	{
		if ((*pieces)[dstPos] == 0 && (*enemyPieces)[dstPos] == 0)
		{
			emptyCount++;
			if (emptyCount >= 2)
			{
				break;
			}
			else
			{
				lineType->Add(0);
			}
		}
		else if ((*pieces)[dstPos] == 1)
		{
			lineType->Add(1);
			emptyCount = 0;
		}
		else if ((*enemyPieces)[dstPos] == 1)
		{
			break;
		}

		posY = posY - dy;
		posX = posX - dx;
		dstPos = (posY * 15) + posX;
		if (posX > 14 || posY > 14 || posX < 0 || posY < 0)
		{
			break;
		}
	}
	
	return lineType;
}

// type 0 : open three, type 1 : close four, type 2 : open four
int State::checkLineTypeSpeical(TArray<int32>* pieces, TArray<int32>* enemyPieces, int pos, int type)
{
	TArray<TArray<int32>*> line_type;
	static TArray<TArray<int32>> speical_line_type[4];
	if (speical_line_type[0].Num() == 0) {
		speical_line_type[0].Add(TArray<int32>({ 0,1,1,1,0 }));
		speical_line_type[0].Add(TArray<int32>({ 0,1,0,1,1,0 }));
		speical_line_type[0].Add(TArray<int32>({ 0,1,1,0,1,0 }));
	
		speical_line_type[1].Add(TArray<int32>({ 0,1,1,1,1 }));
		speical_line_type[1].Add(TArray<int32>({ 1,1,1,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 1,1,1,0,1 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,1,1,0,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,1,1,0,1 }));
		speical_line_type[1].Add(TArray<int32>({ 1,1,1,0,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 1,1,0,1,1 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,1,0,1,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 1,1,0,1,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,1,0,1,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 1,0,1,1,1 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,0,1,1,1,0 }));
		speical_line_type[1].Add(TArray<int32>({ 0,1,0,1,1,1 }));
		speical_line_type[1].Add(TArray<int32>({ 1,0,1,1,1,0 }));
		
		speical_line_type[2].Add(TArray<int32>({ 0,1,1,1,1,0 }));
	}

	line_type.Add(checkLineType(pieces, enemyPieces, pos, -1, 0));
	line_type.Add(checkLineType(pieces, enemyPieces, pos, 0, -1));
	line_type.Add(checkLineType(pieces, enemyPieces, pos, -1, -1));
	line_type.Add(checkLineType(pieces, enemyPieces, pos, -1, 1));

	int count = 0;
	for (int i = 0; i < line_type.Num(); i++) {
		for (int j = 0; j < speical_line_type[type].Num(); j++) {
			if (this->equal_array(line_type[i],&speical_line_type[type][j])) {
				count++;
				break;
			}
		}
	}

	for (int i = 0; i < line_type.Num(); i++) {
		delete line_type[i];
	}

	return count;
}

bool State::checkEmpty(int pos)
{
	int pos_x = pos % 15;
	int pos_y = pos / 15;

	int start_x = pos_x - 1;
	int start_y = pos_y - 1;

	for (int i = 0; i < 9; i++) {
		int dist_pos_x = start_x + (i % 3);
		int dist_pos_y = start_y + (i / 3);

		if (dist_pos_x < 0 || dist_pos_x > 14 || dist_pos_y < 0 || dist_pos_y > 14) {
			continue;
		}

		if ((*this->mPieces)[dist_pos_x + (dist_pos_y * 15)] == 1 ||
			(*this->mEnemyPieces)[dist_pos_x + (dist_pos_y * 15)] == 1) {
			return false;
		}
	}

	return true;
}

bool State::equal_array(TArray<int32>* array1, TArray<int32>* array2)
{
	if (array1->Num() != array2->Num()) {
		return false;
	}

	for (int i = 0; i < array1->Num(); i++) {
		if ((*array1)[i] != (*array2)[i]) {
			return false;
		}
	}

	return true;
}

bool State::isFirstPlayer()
{
	return (this->getPieceCount(this->mPieces) == this->getPieceCount(this->mEnemyPieces));
}

void State::gameReset()
{
	for (TObjectIterator<AGoStone> entity; entity; ++entity)
	{
		entity->Destroy();
	}

	mPieces->Init(0, 225);
	mEnemyPieces->Init(0, 225);
}
