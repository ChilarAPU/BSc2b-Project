// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreen.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndScreen::NativeConstruct()
{
	Super::NativeConstruct();

	QuitButton->OnClicked.AddDynamic(this, &UEndScreen::QuitGame);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly UIOnly;
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(UIOnly);
	PC->SetShowMouseCursor(true);
	
}

void UEndScreen::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}
