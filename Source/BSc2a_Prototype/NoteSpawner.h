// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteSpawner.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API ANoteSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteSpawner();

	//Gets set in the project world to make it easy to add and remove spawn points
	UPROPERTY(EditAnywhere, BlueprintreadWrite)
	TArray<FVector> PossibleSpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> NoteClass;

	//Set in blueprint and will decide how many times to run through SpawnNotes()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfNotesToSpawn;

	//Used to initialize how many spawn locations we want (Must be higher than
	//AmountOfNotesToSpawn otherwise some notes will not spawn and spawning will not be randomized)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfSpawnLocations;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Spawn in multiple note classes into the world while also making sure none of them overlap
	 * NOTE: IF WE HAVE MORE NOTES TO SPAWN THAN SPAWN POSITIONS, THE CODE WILL WILL LOOP FOREVER
	 */
	void SpawnNotes();

};
