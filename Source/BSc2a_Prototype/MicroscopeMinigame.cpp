// Fill out your copyright notice in the Description page of Project Settings.


#include "MicroscopeMinigame.h"
#include "MicroscopeView.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

AMicroscopeMinigame::AMicroscopeMinigame()
{
	PrimaryActorTick.bCanEverTick = true;

	Table = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	Table->SetupAttachment(RootComp);

	Microscope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Microscope"));
	Microscope->SetupAttachment(RootComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComp);

	PlayableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayableArea"));
	PlayableArea->SetupAttachment(Camera);
	
}

void AMicroscopeMinigame::SpawnWidget(APlayerController* PlayerC)
{
	if (IsValid(UI))
	{
		UMicroscopeView* LocalUI = CreateWidget<UMicroscopeView>(PlayerC, UI);
		LocalUI->AddToViewport();
	}
}

