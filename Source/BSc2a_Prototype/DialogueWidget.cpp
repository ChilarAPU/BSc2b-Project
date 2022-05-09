// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Escape->OnClicked.AddDynamic(this, &UDialogueWidget::EscapeClicked);
	EndGameButton->OnClicked.AddDynamic(this, &UDialogueWidget::EndClicked);
	WinButton->OnClicked.AddDynamic(this, &UDialogueWidget::WinClicked);
	
	
	//default values
	DirectPlayerText->SetJustification(ETextJustify::Center);
	bALlowWin = false;
	StringIndex = 0;
	TextSpeed = .05f;
	
}

void UDialogueWidget::EscapeClicked()
{
	//Give Control back to the player and remove the UI from the screen
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameMode;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameMode);
	RemoveFromParent();
}

void UDialogueWidget::EndClicked()
{
	
}

void UDialogueWidget::WinClicked()
{
	if (bALlowWin)
	{
		InputText = "Win";
		//Simple delegate to set off DisplayText() every so many seconds
		TextAnim = UKismetSystemLibrary::K2_SetTimer(this, "DisplayText", TextSpeed, true);
	} else
	{
		InputText = "Complete the medicine to continue";
		TextAnim = UKismetSystemLibrary::K2_SetTimer(this, "DisplayText", TextSpeed, true);
	}
	
}

void UDialogueWidget::DisplayText()
{
	TArray<FString> temp = UKismetStringLibrary::GetCharacterArrayFromString(InputText);
	if (temp.IsValidIndex(StringIndex))
	{
		DisplayedText = DisplayedText + temp[StringIndex];
		DirectPlayerText->SetText(FText::FromString(DisplayedText));
		StringIndex++;
	} else
	{
		//Stop the delegate from going off once the text has been fully displayed
		TextAnim.Invalidate();
	}
	
	
}
