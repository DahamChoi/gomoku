// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoStone.generated.h"

UCLASS()
class TICKTACKTOCK_API AGoStone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void setType(int type);

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BlackStone;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WhiteStone;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RedStone;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OpacityStone;
};
