// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerScreenWidget.h"

#include "CustomPlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Types/SlateEnums.h"

void UComputerScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Exit->OnClicked.AddDynamic(this, &UComputerScreenWidget::ExitComputer);
	Password->OnTextCommitted.AddDynamic(this, &UComputerScreenWidget::PasswordCheck);

	//default value
	TargetPassword = "0123"; //Should get changed in widget blueprint
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
}

void UComputerScreenWidget::PasswordCheck(const FText& InText, ETextCommit::Type InCommit)
{
	const FString Input = InText.ToString();
	if (Input == TargetPassword)
	{
		//Password is correct
		UE_LOG(LogTemp, Warning, TEXT("Correct"));
	} else
	{
		//Password is incorrect
		UE_LOG(LogTemp, Warning, TEXT("Wrong"));
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
