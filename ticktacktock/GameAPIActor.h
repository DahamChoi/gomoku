// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAsyncTask.h"
#include "GoStone.h"
#include "GameAPIActor.generated.h"

UCLASS()
class TICKTACKTOCK_API AGameAPIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameAPIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	FVector ActionToStoneVector(int action);

	UFUNCTION(BlueprintCallable)
	int StoneVectorToAction(FVector vector);

	UFUNCTION(BlueprintCallable)
	bool nextAction(int action);

	UFUNCTION(BlueprintCallable)
	void setTextRender(FString text, FColor color,bool visible = true,float time = -1.f);

	UFUNCTION(BlueprintCallable)
	void AIPlay();

	UFUNCTION(BlueprintCallable)
	void resetGame();

	UFUNCTION(BlueprintCallable)
	bool readyStone(int action);

private:
	FAsyncTask<AIAsyncTask>* AITask;
	int ai_action;

	AGoStone* readyStoneActor;
	TArray<AGoStone*> unvalueStoneArray;
};
