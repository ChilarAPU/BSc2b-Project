// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBaseClass.h"
#include "ComputerMinigame.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API AComputerMinigame : public AInteractableBaseClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComputerMinigame();

	//Does not contain much if any code, Most functionality is inside the ComputerScreenWidget
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ComputerMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* ComputerScreen;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Any custom overlap behaviour goes here. Overriden from InteractableBaseClass
	virtual void CustomOverlapBehaviour(AActor* OtherActor) override;
};
