// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "BSc2a_PrototypeGameModeBase.h"
#include "CustomPlayer.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &UMainMenu::StartGame);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsPage);
	BobbingCheck->OnCheckStateChanged.AddDynamic(this, &UMainMenu::BobbingCheckChange);
}

void UMainMenu::StartGame()
{
	ACustomPlayer* Player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FViewTargetTransitionParams Params;
	Params.BlendTime = 1.f;
	Params.BlendFunction = VTBlend_EaseInOut;
	PlayerC->SetViewTarget(Player, Params);
	RemoveFromParent();
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameMode;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameMode);
}

void UMainMenu::SettingsPage()
{
	if (StartButton->Visibility == ESlateVisibility::Visible)
	{
		StartButton->SetVisibility(ESlateVisibility::Hidden);
		BobbingCheck->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
		BobbingCheck->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UMainMenu::BobbingCheckChange(bool Incoming)
{
	ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->bTiltCameraWhenMoving = Incoming;
}
