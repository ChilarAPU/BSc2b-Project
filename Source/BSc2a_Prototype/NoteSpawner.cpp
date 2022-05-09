// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteSpawner.h"

#include "BaseNoteClass.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANoteSpawner::ANoteSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default values
	AmountOfNotesToSpawn = 0;
	AmountOfSpawnLocations = 5;

	PossibleSpawnLocations.AddUninitialized(AmountOfSpawnLocations);
}

// Called when the game starts or when spawned
void ANoteSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnNotes();
}

// Called every frame
void ANoteSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANoteSpawner::SpawnNotes()
{
	//Holds already spawned in positions to stop notes from spawning on-top of each other
	TArray<int> LocationsSpawnedIn;
	for (int i = 0; i < AmountOfNotesToSpawn; i++)
	{
		int ArrayIndexToGet = UKismetMathLibrary::RandomIntegerInRange(0, PossibleSpawnLocations.Num() - 1);
		//What should happen when a note is going to spawn in a place where one already exists
		while (LocationsSpawnedIn.Contains(ArrayIndexToGet))
		{
			//Get another random index in the array
			ArrayIndexToGet = UKismetMathLibrary::RandomIntegerInRange(0, PossibleSpawnLocations.Num() - 1);
		}
		
		FVector Loc = PossibleSpawnLocations[ArrayIndexToGet];
		FRotator Rot = FRotator(0.f, 0.f, 0.f);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ABaseNoteClass>(NoteClass, Loc, Rot, SpawnParams);

		//adds index into new array so we dont spawn in the same location twice
		LocationsSpawnedIn.Emplace(ArrayIndexToGet);
	}
}


