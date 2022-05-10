// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseNoteClass.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API ABaseNoteClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseNoteClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	//Each note will be hidden unless states otherwise in ShowCorrectNote()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Note1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Note2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Note3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Note4;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Contains switch case that tells the game what note it needs to spawn taking into account how many have already
	 * been spawned in 
	 */
	void ShowCorrectNote();

};
