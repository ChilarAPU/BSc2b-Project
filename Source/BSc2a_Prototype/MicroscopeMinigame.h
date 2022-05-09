// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBaseClass.h"
#include "Camera/CameraComponent.h"
#include "MicroscopeMinigame.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API AMicroscopeMinigame : public AInteractableBaseClass
{
	GENERATED_BODY()

	AMicroscopeMinigame();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Table;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Microscope;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* PlayableArea;

	UFUNCTION()
	void SpawnWidget(APlayerController* PlayerC);

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UMicroscopeView> UI;
	
};
