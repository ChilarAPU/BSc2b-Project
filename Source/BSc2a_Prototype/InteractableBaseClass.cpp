// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBaseClass.h"

#include "CustomPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AInteractableBaseClass::AInteractableBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	CollideTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CollideTrigger"));
	CollideTrigger->SetupAttachment(RootComp);

	InteractText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("InteractText"));
	InteractText->SetupAttachment(RootComp);
	//Set the text to be nothing on construct
	InteractText->SetText(FText());
	InteractText->HorizontalAlignment = EHTA_Center;

	CollideTrigger->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBaseClass::OnOverlapBegin);

	//default values
	InteractString = "Press F To Interact";
	StringIndex = 0;
}

// Called when the game starts or when spawned
void AInteractableBaseClass::BeginPlay()
{
	Super::BeginPlay();

	CollideTrigger->OnComponentEndOverlap.AddDynamic(this, &AInteractableBaseClass::OnOverlapEnd);
}

// Called every frame
void AInteractableBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableBaseClass::OverlapFunction(AActor* OtherActor)
{
	ACustomPlayer* Player = Cast<ACustomPlayer>(OtherActor);
	if (Player)
	{
		Player->InRangeOfInteractable();
	}
}

void AInteractableBaseClass::DisplayText()
{
	TArray<FString> temp = UKismetStringLibrary::GetCharacterArrayFromString(InteractString);
	if (temp.IsValidIndex(StringIndex))
	{
		DisplayedText = DisplayedText + temp[StringIndex];
		InteractText->SetText(FText::FromString(DisplayedText));
		StringIndex++;
	} else
	{
		//Stop the delegate from going off once the text has been fully displayed
		GetWorldTimerManager().ClearTimer(TextAnim);
	}
}

void AInteractableBaseClass::ResetText()
{
	if (DisplayedText.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(ReverseTextAnim);
	} else
	{
		DisplayedText.RemoveAt(DisplayedText.Len());
		DisplayedText.RemoveAt(0);
		InteractText->SetText(FText::FromString(DisplayedText));
	}
}

void AInteractableBaseClass::CustomOverlapBehaviour(AActor* OtherActor)
{
	//Used for child functions
}

void AInteractableBaseClass::CustomOverlapEndBehaviour(AActor* OtherActor)
{
	//Used for child functions
}

void AInteractableBaseClass::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapFunction(OtherActor);
	TextAnim = UKismetSystemLibrary::K2_SetTimer(this, "DisplayText", .02f, true);
	CustomOverlapBehaviour(OtherActor);
}

void AInteractableBaseClass::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapFunction(OtherActor);
	ReverseTextAnim = UKismetSystemLibrary::K2_SetTimer(this, "ResetText", .02f, true);
	StringIndex = 0;
	CustomOverlapEndBehaviour(OtherActor);
}

