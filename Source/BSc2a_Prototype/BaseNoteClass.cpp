// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNoteClass.h"

#include "BSc2a_PrototypeGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseNoteClass::ABaseNoteClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	Note1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note1"));
	Note1->SetupAttachment(RootComp);
	//Change collision profile name to allow it to be picked up by players UPhysicsGrabber
	Note1->SetCollisionProfileName(TEXT("PhysicsActor"));

	Note2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note2"));
	Note2->SetupAttachment(RootComp);
	Note2->SetCollisionProfileName(TEXT("PhysicsActor"));

	Note3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note3"));
	Note3->SetupAttachment(RootComp);
	Note3->SetCollisionProfileName(TEXT("PhysicsActor"));
}

// Called when the game starts or when spawned
void ABaseNoteClass::BeginPlay()
{
	Super::BeginPlay();

	ShowCorrectNote();
}

// Called every frame
void ABaseNoteClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseNoteClass::ShowCorrectNote()
{
	ABSc2a_PrototypeGameModeBase* GM = Cast<ABSc2a_PrototypeGameModeBase>( UGameplayStatics::GetGameMode(GetWorld()));
	//increment the amount spawned variable as we are currently spawning one in
	GM->AmountOfNotesSpawned++;
	switch (GM->AmountOfNotesSpawned)
	{
	case 1:
		Note1->SetHiddenInGame(false);
		break;
	case 2:
		Note2->SetHiddenInGame(false);
		break;
	case 3:
		Note3->SetHiddenInGame(false);
		break;
	default:
		//Used if we try and spawn a fourth or higher note into the world
		UE_LOG(LogTemp, Warning, TEXT("default case"));
		break;
	}
}


