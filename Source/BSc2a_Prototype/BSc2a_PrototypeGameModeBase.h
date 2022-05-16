// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BSc2a_PrototypeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API ABSc2a_PrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//Used to decide which note should be spawned in. Used in ABaseNoteClass.cpp
	int AmountOfNotesSpawned;

	//A accessibility feature as during testing, comments were made about the tilting causing motion sickness
	bool bTiltCameraWhenMoving = true;

	//Easy way to know when the player has completed all mini-games. Amount will always be the same regardless of playthrough
	//Blueprintable to allow davids work to interact with it
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfMinigamesCompleted;

	//Will be used with UMicroscopeMicrobe to decide which microbe image it should show
	int MicrobeValue;

	bool bHasPlayerBeatMicroscope;

	UFUNCTION(BlueprintCallable)
	void AddToMinigameAmount();
};
