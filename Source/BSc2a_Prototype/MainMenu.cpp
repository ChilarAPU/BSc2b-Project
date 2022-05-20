// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "BSc2a_PrototypeGameModeBase.h"
#include "CustomPlayer.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &UMainMenu::StartGame);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsPage);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	BobbingCheck->OnCheckStateChanged.AddDynamic(this, &UMainMenu::BobbingCheckChange);

	//Make Checkbox visually match up with the gamemodes variable
	ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode->bTiltCameraWhenMoving)
	{
		BobbingCheck->SetCheckedState(ECheckBoxState::Checked);
	} else if (GameMode->bTiltCameraWhenMoving == false)
	{
		BobbingCheck->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UMainMenu::StartGame()
{
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	ACustomPlayer* Player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player->bFirstPlayThrough)
	{
		/*TutorialImage->SetVisibility(ESlateVisibility::Visible);
		FTimerHandle OutHandle;
		Player->bFirstPlayThrough = false;
		GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &UMainMenu::StartGameContinued, 5.f, false, 0);
		*/
		TempStartFunction();
	} else
	{
		StartGameContinued();
	}
	
}

void UMainMenu::TempStartFunction()
{
	TutorialImage->SetVisibility(ESlateVisibility::Visible);
	ACustomPlayer* Player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->bFirstPlayThrough = false;
}

void UMainMenu::StartGameContinued()
{
	//TutorialImage->SetVisibility(ESlateVisibility::Hidden);
	ACustomPlayer* Player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	FViewTargetTransitionParams Params;
	Params.BlendTime = 1.f;
	Params.BlendFunction = VTBlend_EaseInOut;
	PlayerC->SetViewTarget(Player, Params);
	RemoveFromParent();
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameMode;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameMode);
	//Allow player to spawn this widget whenever it wants
	Player->MainMenuOpen = false;
}

void UMainMenu::SettingsPage()
{
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
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

void UMainMenu::QuitGame()
{
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UMainMenu::BobbingCheckChange(bool Incoming)
{
	ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->bTiltCameraWhenMoving = Incoming;
}
