// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrabber::Grab()
{
	AActor* HitActor;
	UPrimitiveComponent* HitComponent;
	if (CustomTraceForPhysicsBodies(HitActor, HitComponent))
	{
		HitComponent->SetSimulatePhysics(true);
		GetPhysicsComponent()->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitComponent->GetCenterOfMass(),
			FRotator()
			);
	}
}

void UGrabber::Release()
{
	GetPhysicsComponent()->ReleaseComponent();
	//To optimise this, stop simulating physics a few seconds after the release button has been pressed
}

bool UGrabber::CustomTraceForPhysicsBodies_Implementation(AActor*& HitActor, UPrimitiveComponent*& HitComponent)
{
	FHitResult OutHit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	TArray<AActor*> IgnoredActors;
	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetComponentLocation(), GetMaxGrabLocation(), 50.f, ObjectTypesArray, false,
		IgnoredActors, EDrawDebugTrace::None, OutHit, true);
	if (hit)
	{
		HitActor = OutHit.GetActor();
		HitComponent = OutHit.GetComponent();
		return true;
	} else
	{
		return false;
	}
	
}



// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetPhysicsComponent()->SetTargetLocationAndRotation(GetHoldLocation(), GetComponentRotation());
}

