// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "BSc2a_PrototypeGameModeBase.h"
#include "EndScreen.h"
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
	bFinishEarly = false;

	if (EndScreen)
	{
		EndScreenRef= Cast<UEndScreen>(CreateWidget(GetWorld(), EndScreen));
	}
	
}

void UDialogueWidget::EscapeClicked()
{
	//Give Control back to the player and remove the UI from the screen
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameMode;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameMode);
	RemoveFromParent();
}

void UDialogueWidget::EndClicked()
{
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (EndScreenRef)
	{
		EndScreenRef->AddToViewport();
		EndScreenRef->LoseScreen->SetVisibility(ESlateVisibility::Visible);
		FString EndingText = "How could one person, save the entire world? It was too late to think about that, the disease had progressed too far already and all that was left was to simply just try. But try as one might, in the end, it turned out to be too great of a responsibility for one. Whether it was lack of skill, cracking under the pressure or simply not being quick enough, ultimately the cure wasn’t created. There was nothing anyone could have done to change this. By now, resources and efforts had been exhausted, and the last chance missed. All one could do now, was wait for the impending destruction…";
		EndScreenRef->EndingText->SetText(FText::FromString(EndingText));
	}
	
}

void UDialogueWidget::WinClicked()
{
	if (ButtonClickSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	ABSc2a_PrototypeGameModeBase* GM = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (bALlowWin)
	{
		if (EndScreenRef)
		{
			EndScreenRef->AddToViewport();
			EndScreenRef->WinScreen->SetVisibility(ESlateVisibility::Visible);
			FString EndingText = "In a time were the whole world crumbled and lost all hope, one person undertook a great responsibility. By taking it upon themselves, they now held the future of mankind in their hands, but rather than giving into the pressure that rested on their shoulders, they took it and worked with it. With the cries and grunts of pain a constant reminder, they successfully created the cure. A small vial, one would find it hard to believe that within it, rested the cure to what could have been the end of the human race…";
			EndScreenRef->EndingText->SetText(FText::FromString(EndingText));
		}
		
		//Simple delegate to set off DisplayText() every so many seconds
		TextAnim = UKismetSystemLibrary::K2_SetTimer(this, "DisplayText", TextSpeed, true);
	} else if (GM->AmountOfMinigamesCompleted > 0)
	{
		/*EndScreenRef->AddToViewport();
		EndScreenRef->LoseScreen->SetVisibility(ESlateVisibility::Visible);
		FString EndingText = "The efforts put into creating the cure were incomparable to anything else. Truly, the cure was in reaching distance, so close to being held and used for the greater good. Yet, the pressure was too much, and the surrounding destruction caused by the apocalypse was getting out of hand. Time was not a luxury anyone had, especially when creating the cure, and so, everything that had to be done, ended up being rushed. What seemed to be the medicine for all the problems in the world, actually turned out to be what made it worse. Like fuel to the fire, the rushed cure that was produced only helped in the progression of the apocalypse, creating new strains, arguably more deadly and violent than the previous…";
		EndScreenRef->EndingText->SetText(FText::FromString(EndingText));
		*/
		if (bFinishEarly)
		{
			EndScreenRef->AddToViewport();
			EndScreenRef->LoseScreen->SetVisibility(ESlateVisibility::Visible);
			FString EndingText = "The efforts put into creating the cure were incomparable to anything else. Truly, the cure was in reaching distance, so close to being held and used for the greater good. Yet, the pressure was too much, and the surrounding destruction caused by the apocalypse was getting out of hand. Time was not a luxury anyone had, especially when creating the cure, and so, everything that had to be done, ended up being rushed. What seemed to be the medicine for all the problems in the world, actually turned out to be what made it worse. Like fuel to the fire, the rushed cure that was produced only helped in the progression of the apocalypse, creating new strains, arguably more deadly and violent than the previous…";
			EndScreenRef->EndingText->SetText(FText::FromString(EndingText));
		} else
		{
			InputText = "There are still tasks left to complete the medicine, Are you sure you want to continue?";
			bFinishEarly = true;
			TextAnim = UKismetSystemLibrary::K2_SetTimer(this, "DisplayText", TextSpeed, true);
		}
		
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
