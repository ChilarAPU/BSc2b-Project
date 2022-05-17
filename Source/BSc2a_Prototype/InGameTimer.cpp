// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTimer.h"

#include "EndScreen.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AInGameTimer::AInGameTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	
	ClockBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBody"));
	ClockBody->SetupAttachment(GetRootComponent());
	
	TimerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimerText"));
	TimerText->SetupAttachment(ClockBody);

	//Default values
	TimeRemaining = 0; //should be overridden in blueprints
	TimeRemainingMinutes = 0;
	TimeRemainingSeconds = 0;
	bShouldClockBeCounting = true;

}

// Called when the game starts or when spawned
void AInGameTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInGameTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldClockBeCounting)
	{
		SetTimerText(DeltaTime);
	}
	

}

void AInGameTimer::SetTimerText(float DeltaTime)
{
	//Smoothly transition between one number to the next
    TimeRemaining = FMath::FInterpTo(TimeRemaining, TimeRemaining - 1, DeltaTime, 1.f );
    //Going to be using a digital clock to showcase the time left so I dont want a smooth transition between numbers
	//Split the TimeRemaining into minutes and seconds
    TimeRemainingMinutes = FMath::RoundToZero(TimeRemaining / 60);
    TimeRemainingSeconds = TimeRemaining - (TimeRemainingMinutes * 60);
	//Display it onto the text widget
    TimerText->SetText(FText(FText::FromString(FString(FString::FromInt(TimeRemainingMinutes)) + " : " + FString::FromInt(TimeRemainingSeconds))));
	
	if (TimeRemainingMinutes == 0 && TimeRemainingSeconds == 0)
	{
		if (LoseScreen)
		{
			UEndScreen* Local = Cast<UEndScreen>(CreateWidget(GetWorld(), LoseScreen));
			Local->AddToViewport();
			Local->LoseScreen->SetVisibility(ESlateVisibility::Visible);
			bShouldClockBeCounting = false;
			FString EndText = "How could one person, save the entire world? It was too late to think about that, the disease had progressed too far already and all that was left was to simply just try. But try as one might, in the end, it turned out to be too great of a responsibility for one. Whether it was lack of skill, cracking under the pressure or simply not being quick enough, ultimately the cure wasn’t created. There was nothing anyone could have done to change this. By now, resources and efforts had been exhausted, and the last chance missed. All one could do now, was wait for the impending destruction…";
			Local->EndingText->SetText(FText::FromString(EndText));
		}
		
	}
}

