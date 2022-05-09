// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGoal.h"



#include "CustomPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEndGoal::AEndGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ComputerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ComputerMesh"));
	ComputerMesh->SetupAttachment(RootComp);
	
}

// Called when the game starts or when spawned
void AEndGoal::BeginPlay()
{
	Super::BeginPlay();
	//Binding this in the constructor does not seem to work. This is a workaround
	
}

// Called every frame
void AEndGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndGoal::CustomOverlapBehaviour(AActor* OtherActor)
{
	Super::CustomOverlapBehaviour(OtherActor);
	
}



