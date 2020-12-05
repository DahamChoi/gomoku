// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ticktacktockPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"
#include "UObject/ConstructorHelpers.h"
#include "TickTakTokGameStateBase.h"
#include "GoStone.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "TimerManager.h"
#include "Node.h"
#include "GameAPI.h"
#include "Components/TextRenderComponent.h"

AticktacktockPawn::AticktacktockPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AITask = nullptr;
}

void AticktacktockPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AticktacktockPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AticktacktockPawn::TriggerClick);
}

void AticktacktockPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AticktacktockPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AticktacktockPawn::TriggerClick()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
//		TraceForBlock(Start, End, false);
	}
}

void AticktacktockPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	/*
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams::AllDynamicObjects);
	ATickTakTokGameStateBase * gameState = GetWorld()->GetGameState<ATickTakTokGameStateBase>();

	if (HitResult.Actor.IsValid())
	{
		ATriggerVolume * HitBlock = Cast<ATriggerVolume>(HitResult.Actor.Get());
		if (HitBlock) 
		{
			if (gameState->state->isDone())
			{
				return;
			}

			FVector actorLocation = HitBlock->GetActorLocation();

			switch (gameState->gameStateId)
			{
				// OFFLINE
			case 0:
				GameAPI::nextAction(GameAPI::StoneVectorToAction(actorLocation),GetWorld());
				break;

				// ONLINE
			case 1:

				break;

				// AI
			case 2:
				if (GameAPI::nextAction(GameAPI::StoneVectorToAction(actorLocation),GetWorld())) {

					// AI Task
					GameAPI::setTextRender(GetWorld(), TEXT("AI TURN..."), FColor::FColor(255, 255, 255));
					AITask = new FAsyncTask<AIAsyncTask>(&this->ai_action,GetWorld());
					AITask->StartBackgroundTask();
				}
				break;
			}
		}
	}
	*/
}