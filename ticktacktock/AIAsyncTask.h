// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Engine/World.h"

/**
 * 
 */
class TICKTACKTOCK_API AIAsyncTask : public FNonAbandonableTask
{
	friend class FAsyncTask<AIAsyncTask>;
	
public:
	AIAsyncTask(int * action,UWorld * world);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(AIAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	int * ai_action;
	UWorld* world;
};
