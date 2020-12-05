// Fill out your copyright notice in the Description page of Project Settings.


#include "GoStone.h"
#include "Engine/StaticMesh.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "TickTakTokGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGoStone::AGoStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlackStone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BLACK_STONE"));
	WhiteStone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WHITE_STONE"));
	RedStone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RED_STONE"));
	OpacityStone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OPACITY_STONE"));

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	BlackStone->SetupAttachment(DummyRoot);
	WhiteStone->SetupAttachment(DummyRoot);
	RedStone->SetupAttachment(DummyRoot);
	OpacityStone->SetupAttachment(DummyRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BLACK_STONE(
		TEXT("/Game/GoSet/Meshes/GoStone/SM_GoStoneBlack_size2.SM_GoStoneBlack_size2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WHITE_STONE(
		TEXT("/Game/GoSet/Meshes/GoStone/SM_GoStoneWhite_size2.SM_GoStoneWhite_size2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RED_STONE(
		TEXT("/Game/GoSet/Meshes/GoStone/SM_GoStoneRed_size2.SM_GoStoneRed_size2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_OPACITY_STONE(
		TEXT("/Game/GoSet/Meshes/GoStone/SM_GoStoneOpacity_size2.SM_GoStoneOpacity_size2"));

	if (SM_BLACK_STONE.Succeeded())
	{
		BlackStone->SetStaticMesh(SM_BLACK_STONE.Object);
		BlackStone->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		BlackStone->SetVisibility(false);
	}

	if (SM_WHITE_STONE.Succeeded())
	{
		WhiteStone->SetStaticMesh(SM_WHITE_STONE.Object);
		WhiteStone->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		WhiteStone->SetVisibility(false);
	}

	if (SM_RED_STONE.Succeeded())
	{
		RedStone->SetStaticMesh(SM_RED_STONE.Object);
		RedStone->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		RedStone->SetVisibility(false);
	}

	if (SM_OPACITY_STONE.Succeeded()) 
	{
		OpacityStone->SetStaticMesh(SM_OPACITY_STONE.Object);
		OpacityStone->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		OpacityStone->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void AGoStone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoStone::setType(int type)
{
	// Black : FirstPlayer
	if (type == 0) 
	{
		BlackStone->SetVisibility(true);
	}
	// White : SecondPlayer
	else if(type == 1)
	{
		WhiteStone->SetVisibility(true);
	}
	else if(type == 2)
	{
		RedStone->SetVisibility(true);
	}
	else
	{
		OpacityStone->SetVisibility(true);
	}
}

