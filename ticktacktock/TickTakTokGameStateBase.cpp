// Fill out your copyright notice in the Description page of Project Settings.


#include "TickTakTokGameStateBase.h"
#include "GameAPI.h"

ATickTakTokGameStateBase::ATickTakTokGameStateBase()
{
	state = new State();
}

ATickTakTokGameStateBase::~ATickTakTokGameStateBase()
{
	delete state;
}

bool ATickTakTokGameStateBase::isGameDone()
{
	return this->state->isDone();
}

bool ATickTakTokGameStateBase::isFirstPlayer()
{
	return this->state->isFirstPlayer();
}