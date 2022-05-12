// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuCamera.h"

#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainMenuCamera::AMainMenuCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMainMenuCamera::BeginPlay()
{
	Super::BeginPlay();

	//Blend from player to this classes camera at beginning of level.
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FViewTargetTransitionParams Params;
	PlayerC->SetViewTarget(this, Params);
	if (MainMenuWidget)
	{
		UMainMenu* MainMenuRef = Cast<UMainMenu>(CreateWidget(GetWorld(), MainMenuWidget));
		if (MainMenuRef != nullptr)
		{
			MainMenuRef->AddToViewport();
			PlayerC->SetIgnoreMoveInput(true);
			FInputModeUIOnly MMOpen;
			MMOpen.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerC->SetShowMouseCursor(true);
			PlayerC->SetInputMode(MMOpen);
		}
		
	}
}

// Called every frame
void AMainMenuCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

