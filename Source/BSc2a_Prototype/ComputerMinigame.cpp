// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerMinigame.h"

#include "CustomPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AComputerMinigame::AComputerMinigame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComputerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Computer"));
	ComputerMesh->SetupAttachment(RootComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComp);

	ComputerScreen = CreateDefaultSubobject<UWidgetComponent>(TEXT("ComputerScreen"));
	ComputerScreen->SetupAttachment(RootComp);
	
	
}

// Called when the game starts or when spawned
void AComputerMinigame::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComputerMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AComputerMinigame::CustomOverlapBehaviour(AActor* OtherActor)
{
	Super::CustomOverlapBehaviour(OtherActor);
	ACustomPlayer* Player = Cast<ACustomPlayer>(OtherActor);
	if (Player)
	{
		
	}
}


