// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BSC2A_PROTOTYPE_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMaxGrabLocation() const
	{
		//GetComponentRotation().Vector() returns the forward vector
		return GetComponentLocation() + GetComponentRotation().Vector() * MaxGrabDistance;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetHoldLocation() const
	{
		return GetComponentLocation() + GetComponentRotation().Vector() * HoldDistance;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPhysicsHandleComponent* GetPhysicsComponent() const
	{
		return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CustomTraceForPhysicsBodies(AActor*& HitActor, UPrimitiveComponent*& HitComponent);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxGrabDistance = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HoldDistance = 100;

		
};
