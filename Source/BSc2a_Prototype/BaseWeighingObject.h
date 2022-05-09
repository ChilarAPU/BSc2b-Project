// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeighingMinigame.h"
#include "GameFramework/Actor.h"
#include "BaseWeighingObject.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API ABaseWeighingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeighingObject();

	//The static mesh which will be visible
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UStaticMeshComponent* Object;

	//Weight of the object, in grams
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight;

	//Used to know when this object is currently being held by the player
	bool bBeingClicked;

	//Used when placing the object back down on the table, this will the object to be placed on any surface within an
	//acceptable amount
	bool bHitObject;

	UPROPERTY()
	APlayerController* PlayerC;

	FHitResult OutHit;

	//Used to access class relevant functions outside of OnHit()
	UPROPERTY()
	AWeighingMinigame* WeightMinigame;

	//Used to know when weight should be removed from the scale
	bool bRemoveWeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Called in tick to reset both the objects position to be sitting back on a surface as well as resetting the scales counter when an object has
	//been picked up
	void ResetLocationAndScale();
	

};
