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
	
};
