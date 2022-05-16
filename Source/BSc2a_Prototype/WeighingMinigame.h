// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBaseClass.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Containers/Map.h"
#include "WeighingMinigame.generated.h"

/**
 * 
 */
//Unused struct
USTRUCT()
struct FTableOfObjectWeight
{
	GENERATED_BODY()

	//Constructor
	FTableOfObjectWeight()
	{
		ObjectType = nullptr;
		TotalObjectWeight = 0;
	}
public:
	UPROPERTY()
	UClass* ObjectType;

	UPROPERTY()
	float TotalObjectWeight;
};

UCLASS()
class BSC2A_PROTOTYPE_API AWeighingMinigame : public AInteractableBaseClass
{
	GENERATED_BODY()

	AWeighingMinigame();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Table;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Scale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ResetScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTextRenderComponent* ScaleAmount;

	//Holds the total weight currently on the scale
	float TotalWeight;

	//Holds the total weight of each individual object on the scale
	UPROPERTY()
	TMap<UClass*, float> TableOfObjectWeight;

	//Holds the goal of the total weight of individual objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UClass*, float> GoalObjectWeight;

	//text timeline specific
	UFUNCTION()
	void TextTimelineProgress(float Value);
	
	FTimeline TextTimeline;
	//Curve used for adding weight
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* TextFloat;

	//Curve used for Removing weight
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* RemoveTextFloat;


	UPROPERTY()
	float StartValue;
	UPROPERTY()
	float EndValue;

	UFUNCTION()
	void ResetCamera(class ACustomPlayer* IncomingPlayer);

	//Used to know how much weight to remove when we remove an object from the scale halfway through the timeline
	float TimelineValue;
	//end of timeline specific
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//Runs timeline to add weight to the scale
	void ChangeScaleAmount(float Weight, UClass* IncomingClass);

	//Runs the timeline to remove weight from the scale
	void RemoveWeight(float Weight, UClass* IncomingClass);
};
