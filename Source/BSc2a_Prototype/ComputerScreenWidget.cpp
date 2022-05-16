// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerScreenWidget.h"

#include "BSc2a_PrototypeGameModeBase.h"
#include "CustomPlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Types/SlateEnums.h"

void UComputerScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Exit->OnClicked.AddDynamic(this, &UComputerScreenWidget::ExitComputer);
	Password->OnTextCommitted.AddDynamic(this, &UComputerScreenWidget::PasswordCheck);

	//default value
	TargetPassword = "0123"; //Should get changed in widget blueprint

	LoadingIcon->SetVisibility(ESlateVisibility::Hidden);
	WelcomeText->SetVisibility(ESlateVisibility::Hidden);
	IncorrectText->SetVisibility(ESlateVisibility::Hidden);
}

FReply UComputerScreenWidget::NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SetImagePositionToMouse(MyGeometry, MouseEvent);
	
	return Super::NativeOnMouseMove(MyGeometry, MouseEvent);
	
}

void UComputerScreenWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}


void UComputerScreenWidget::ExitComputer()
{
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	ACustomPlayer* PlayerPawn= Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//Changing the game mode to allow the player normal control again
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameOpen;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameOpen);
	
	PlayerPawn->ChangeViewTarget(PlayerPawn, PlayerC);

	ABSc2a_PrototypeGameModeBase* GM = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->AddToMinigameAmount();
}

void UComputerScreenWidget::IncorrectDisapear()
{
	IncorrectText->SetVisibility(ESlateVisibility::Hidden);
}

void UComputerScreenWidget::PasswordCheck(const FText& InText, ETextCommit::Type InCommit)
{
	const FString Input = InText.ToString();
	//Even if onenter is the one we want, this would cause 2 events to be run at the same time
	if (InCommit == ETextCommit::Type::OnEnter)
	{
		return;
	}
	if (Input == TargetPassword)
	{
		//Password is correct
		Exit->SetVisibility(ESlateVisibility::Hidden);
		Password->SetVisibility(ESlateVisibility::Hidden);
		LoadingIcon->SetVisibility(ESlateVisibility::Visible);
		WelcomeText->SetVisibility(ESlateVisibility::Visible);
		ExitComputer();
	} else
	{
		//Password is incorrect
		IncorrectText->SetVisibility(ESlateVisibility::Visible);
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UComputerScreenWidget::IncorrectDisapear, 2.f, false);
	}
}

void UComputerScreenWidget::SetImagePositionToMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//Treat the cursor image as a canvas panel so its position can be changed
	UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(ComputerCursor);
	//Set the position of the image to the players cursor
	CanvasPanel->SetPosition(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()));
	//Hide the mouse cursor as we dont want two on the screen at once
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}
