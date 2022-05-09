// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBaseClass.h"
#include "EndGoal.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API AEndGoal : public AInteractableBaseClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndGoal();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ComputerMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CustomOverlapBehaviour(AActor* OtherActor) override;
	
	
};
