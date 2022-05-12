// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InGameTimer.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API AInGameTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInGameTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ClockBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextRenderComponent* TimerText;

	//Used to hold the overall time remaining that then gets split up into seconds and minutes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRemaining;

	int TimeRemainingSeconds;
	int TimeRemainingMinutes;

	bool bShouldClockBeCounting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEndScreen> LoseScreen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTimerText(float DeltaTime);

};
