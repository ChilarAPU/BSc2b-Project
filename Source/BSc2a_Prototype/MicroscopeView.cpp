// Fill out your copyright notice in the Description page of Project Settings.


#include "MicroscopeView.h"

#include "CustomPlayer.h"
#include "MicroscopeBackground.h"
#include "MicroscopeMicrobe.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UMicroscopeView::NativeConstruct()
{
	Super::NativeConstruct();


	//Default values
	ClearValue = UKismetMathLibrary::RandomFloatInRange(0, 1);
	
	//Need to call it once on construct otherwise it will always start of as clear
	OnBlurValueChange(BlurSlider->Value);

	//Binding delegates
	BlurSlider->OnValueChanged.AddDynamic(this, &UMicroscopeView::OnBlurValueChange);

	ExitButton->OnClicked.AddDynamic(this, &UMicroscopeView::ExitView);

	//UCanvasPanelSlot* CP = UWidgetLayoutLibrary::SlotAsCanvasSlot(MainView);
	//MaxSize = CP->GetSize();

	AmountOfMicrobesToSpawn = 10;
	MicrobesRemoved = 0;
	bMouseHeld = false;
	
	for (int i = 0; i < AmountOfMicrobesToSpawn; i++)
	{
		UMicroscopeMicrobe* NewMicrobe = CreateWidget<UMicroscopeMicrobe>(this, Microbe->GetClass());
		NewMicrobe->AddToViewport();
		//temp way to create a bad microbe
		if (i == 5)
		{
			NewMicrobe->bIsBad = true;
		}
		NewMicrobe->IsMicrobeBad(this);
		AllMicrobes.Emplace(NewMicrobe);
	}
	//Create a new background image below the microscope view and the microbes. Have to do this as we cant move this
	//below the microbes without changing its ZOrder in the viewport. Already created image is not seen
	//and only used for mouse button events
	if (BackgroundImage)
	{
		UMicroscopeBackground* BackgroundImageDup = CreateWidget<UMicroscopeBackground>(this, BackgroundImage->GetClass());
    	BackgroundImageDup->AddToViewport(-1);
		BackgroundImageDup->SetParentWidget(this);
		
	}
	
	
	

}

void UMicroscopeView::OnBlurValueChange(float InValue)
{
	float tempValue = ClearValue - InValue;
	//If statement forces the result to be positive as blue strength cannot go below 0
	if (tempValue < 0)
	{
		tempValue *= -1;
	}
	//Times it by a number as otherwise the effect would not be effective enough
	tempValue *= 20;
	Blur->SetBlurStrength(tempValue);
}

void UMicroscopeView::ExitView()
{
	ResetView();
	//currently just removes it from view. Would like to change it so that insted of deleting it, I hide
	//the viewport so that if the player comes back, everything is still how it was.
}

void UMicroscopeView::ResetView()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	ACustomPlayer* PlayerPawn= Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//Changing the game mode to allow the player normal control again
	PlayerC->SetIgnoreMoveInput(false);
	FInputModeGameOnly GameOpen;
	PlayerC->SetShowMouseCursor(false);
	PlayerC->SetInputMode(GameOpen);
	
	PlayerPawn->ChangeViewTarget(PlayerPawn, PlayerC);
}

FReply UMicroscopeView::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	bMouseHeld = true;
	//UE_LOG(LogTemp, Warning, TEXT("DOWN"));
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMicroscopeView::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//UE_LOG(LogTemp, Warning, TEXT("UP"));
	bMouseHeld = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UMicroscopeView::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bMouseHeld)
	{
		if (UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) > MousePos)
		{
			UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(MainView);
			FVector2D temp2 = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MousePos;
			UE_LOG(LogTemp, Warning, TEXT("%f, %f"), temp2.X, temp2.Y);
			FVector2D temp = CanvasPanel->GetPosition();
			CanvasPanel->SetPosition((temp + temp2) / 10);
		}
		
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UMicroscopeView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
