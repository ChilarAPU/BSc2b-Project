// Fill out your copyright notice in the Description page of Project Settings.


#include "WeighingMinigame.h"

#include "BaseWeighingObject.h"
#include "BSc2a_PrototypeGameModeBase.h"
#include "CustomPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

AWeighingMinigame::AWeighingMinigame()
{
	PrimaryActorTick.bCanEverTick = true;

	Table= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	Table->SetupAttachment(RootComp);

	Scale = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scale"));
	Scale->SetupAttachment(RootComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComp);

	ResetScale = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResetScale"));
	ResetScale->SetupAttachment(RootComp);

	ScaleAmount = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScaleAmount"));
	ScaleAmount->SetupAttachment(RootComp);
	
	//default values
	TotalWeight = 0;
	TimelineValue = 1;
}

void AWeighingMinigame::TextTimelineProgress(float Value)
{
	float NewWeight = FMath::Lerp(StartValue, EndValue, Value);
	NewWeight = FMath::Floor(NewWeight * 100) / 100;
	FString temp = FString::SanitizeFloat(NewWeight);
	ScaleAmount->SetText(FText::FromString(temp));
	TotalWeight = NewWeight;
	TimelineValue = Value;
}

void AWeighingMinigame::ResetCamera(ACustomPlayer* IncomingPlayer)
{
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	ACustomPlayer* PlayerPawn= Cast<ACustomPlayer>(IncomingPlayer);
	//Changing the game mode to allow the player normal control again
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameOpen;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameOpen);
	
	PlayerPawn->ChangeViewTarget(PlayerPawn, PlayerC);
}

void AWeighingMinigame::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeighingMinigame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TextTimeline.TickTimeline(DeltaSeconds);
}

void AWeighingMinigame::ChangeScaleAmount(float Weight, UClass* IncomingClass)
{
	//Stop the timeline if it is not already
	TextTimeline.Stop();
	
	if (TextFloat)
	{
		//Add class into TMap or replace it if already exists. Used to know the weight of specific ingredients.
		
		/* This is basically my own implementation of a TMultiMap except that I add the new value with the old value
		 * instead of just replacing it
		 */
		if (TableOfObjectWeight.Contains(IncomingClass))
		{
			float i = 0;
			TableOfObjectWeight.RemoveAndCopyValue(IncomingClass, i);
			TableOfObjectWeight.Add(IncomingClass, Weight + i);
		} else
		{
			TableOfObjectWeight.Add(IncomingClass, Weight);
		}

		//
		if (TableOfObjectWeight[IncomingClass] == GoalObjectWeight[IncomingClass])
		{
			TArray<AActor*> temp;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), IncomingClass, temp);
			GoalObjectWeight.FindAndRemoveChecked(IncomingClass);
			//Removes all actors of the class from the level. This shows to the player that they have got the amount correct
			for (int i = 0; i < temp.Num(); i++)
			{
				ABaseWeighingObject* Ob = Cast<ABaseWeighingObject>(temp[i]);
				Ob->bCanBePickedUp = true;
			}
		}

		//Check whether or not we still need more ingredients 
		TArray<UClass*> temp2;
		GoalObjectWeight.GetKeys(temp2);
		if (temp2.Num() == 0)
		{
			ACustomPlayer* PL = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			ResetCamera(PL);
			ABSc2a_PrototypeGameModeBase* GM = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			GM->AddToMinigameAmount();
		}
		
		//Timeline specific functionality
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TextTimelineProgress"));
		TextTimeline.AddInterpFloat(TextFloat, TimelineProgress);
		TextTimeline.SetLooping(false);
		//Used to get the amount needed to add onto EndValue if we put on a new object before timeline reaches 1
		float BeforeFinish = EndValue * TimelineValue;
		//Used to get the amount needed to remove from EndValue if we put on a new object after timeline reaches 1
		float AfterFinish = TotalWeight - EndValue;
		StartValue = TotalWeight;
		if (TimelineValue < 1)
		{
			EndValue = TotalWeight + Weight + BeforeFinish;
		} else
		{
			EndValue = TotalWeight + Weight - AfterFinish;
		}
		
		TextTimeline.PlayFromStart();
		
		
	}
}

void AWeighingMinigame::RemoveWeight(float Weight, UClass* IncomingClass)
{
	TextTimeline.Stop();
	
	if (TextFloat)
	{
		if (TableOfObjectWeight.Contains(IncomingClass))
		{
			float i = 0;
			TableOfObjectWeight.RemoveAndCopyValue(IncomingClass, i);
			TableOfObjectWeight.Add(IncomingClass, i - Weight);
		}
		
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TextTimelineProgress"));
		TextTimeline.AddInterpFloat(TextFloat, TimelineProgress);
		TextTimeline.SetLooping(false);
		//Used to get the amount needed to remove from EndValue if we put on a new object after timeline reaches 1
		float OldWeight = EndValue - TotalWeight;
		StartValue = TotalWeight;
		EndValue = TotalWeight - Weight + OldWeight;
		TextTimeline.PlayFromStart();
	}
}
