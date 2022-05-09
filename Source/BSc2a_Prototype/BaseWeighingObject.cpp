// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeighingObject.h"

#include "WeighingMinigame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseWeighingObject::ABaseWeighingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	SetRootComponent(Object);

	//default values
	Weight = 0;
	bBeingClicked = false;
	bRemoveWeight = false;
}

// Called when the game starts or when spawned
void ABaseWeighingObject::BeginPlay()
{
	Super::BeginPlay();
	//Used in tick to get the hit result under the cursor
	PlayerC = UGameplayStatics::GetPlayerController(this, 0);

	//Binding this delegate does not work in constructor
	Object->OnComponentHit.AddDynamic(this, &ABaseWeighingObject::OnHit);
	
}

// Called every frame
void ABaseWeighingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ResetLocationAndScale();
}

void ABaseWeighingObject::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	
	//This cast gets used in tick to remove the weight once object has been moved away
	AWeighingMinigame* Minigame = Cast<AWeighingMinigame>(OtherActor);
	if (IsValid(Minigame))
	{
		WeightMinigame = Cast<AWeighingMinigame>(OtherActor);
		bHitObject = true;
		if (OtherComp == Minigame->Scale)
		{
			//Object has been placed on the scale and we want to add weight onto the scale
			Minigame->ChangeScaleAmount(Weight, GetClass());
			//set remove weight to be true so that when we next pick it up, we know we want to remove it
			bRemoveWeight = true;
		}
	}
}

void ABaseWeighingObject::ResetLocationAndScale()
{
	//Is player currently holding the object
	if (bBeingClicked)
	{
		PlayerC->GetHitResultUnderCursor(ECC_Visibility, true, OutHit);
		if (OutHit.bBlockingHit && bRemoveWeight && IsValid(WeightMinigame))
		{
			WeightMinigame->RemoveWeight(Weight, GetClass());
			bRemoveWeight = false;
		}
		SetActorLocation(FVector(OutHit.Location.X, OutHit.Location.Y, GetActorLocation().Z), true);
	} //if not, is the object currently hitting an object
	else if (!bHitObject)
	{
		//if not, push the object down until it is touching an object
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 2), true);
	}
}


