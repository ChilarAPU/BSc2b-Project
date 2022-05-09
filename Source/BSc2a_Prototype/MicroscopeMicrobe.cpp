// Fill out your copyright notice in the Description page of Project Settings.


#include "MicroscopeMicrobe.h"

#include "MicroscopeView.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"


void UMicroscopeMicrobe::NativeConstruct()
{
	Super::NativeConstruct();

	MicrobeDirection = 0;
	AmountToMoveX =  .1;
	AmountToMoveY = .1;
	MoveIncrement = 0;
	ChangeDirectionSpeed = UKismetMathLibrary::RandomIntegerInRange(200, 400);
	bIsBad = false;

	Microbe->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");
	
}

void UMicroscopeMicrobe::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(Microbe);
	FVector2D temp = CanvasPanel->GetPosition();
	MaxSize = MyGeometry.GetLocalSize();
	
	if (CanvasPanel->GetPosition().X > MaxSize.X)
	{
		MicrobeDirection = 1;
	}
	else if (CanvasPanel->GetPosition().X < 0)
	{
		MicrobeDirection = 0;
	}
	
	if (CanvasPanel->GetPosition().Y > MaxSize.Y)
	{
		MicrobeDirection = 1;
	}
	else if (CanvasPanel->GetPosition().Y < 0)
	{
		MicrobeDirection = 0;
	}
	

	if (MoveIncrement == ChangeDirectionSpeed)
	{
		MicrobeDirection = UKismetMathLibrary::RandomIntegerInRange(0, 1);
		AmountToMoveX =  UKismetMathLibrary::RandomFloatInRange(0, .5);
		AmountToMoveY =  UKismetMathLibrary::RandomFloatInRange(0, .5);
		MoveIncrement = 0;
	}
	
	MoveIncrement++;
	
	if (MicrobeDirection == 0)
	{
		CanvasPanel->SetPosition(FVector2D(temp.X + AmountToMoveX, temp.Y + AmountToMoveY));
	} else if (MicrobeDirection == 1)
	{
		CanvasPanel->SetPosition(FVector2D(temp.X - AmountToMoveX, temp.Y - AmountToMoveY));
	}
}

void UMicroscopeMicrobe::MicrobePress(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (bIsBad && IsValid(Parent))
	{
		Microbe->SetOpacity(0);
		Parent->MicrobesRemoved++;
		if (Parent->MicrobesRemoved == 1)
		{
			Parent->ResetView();
		}
	}
}

void UMicroscopeMicrobe::IsMicrobeBad(UMicroscopeView* OwningWidget)
{
	if (bIsBad)
	{
		Microbe->SetColorAndOpacity(FLinearColor::Red);
	}
	//Need to set the spawn handling on a small delay as otherwise, it does not register the viewport
	//as being spawned in
	Parent = Cast<UMicroscopeView>(OwningWidget);
	FTimerHandle TimerHandler;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SetRandomSpawnPoint"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDel, 0.01f, false);
}

void UMicroscopeMicrobe::MoveMicrobes(FVector2D MousePosition)
{
	if (!bFirstRunThrough)
	{
		FVector2D CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		FVector2D t = (CurrentMousePosition - MousePosition) / 20;
		UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(Microbe);
		FVector2D temp = CanvasPanel->GetPosition();
		//This check stops the images from teleporting to the mouse position as well as limiting the speed
		if (t < FVector2D(5, 5) && t > FVector2D(-5, -5))
		{
			CanvasPanel->SetPosition(temp + t);
		}
	}
	
}

void UMicroscopeMicrobe::SetRandomSpawnPoint()
{
	if (IsValid(Parent))
	{
		UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(Microbe);
		CanvasPanel->SetPosition(FVector2D(UKismetMathLibrary::RandomIntegerInRange(0, Parent->GetDesiredSize().X), 
		UKismetMathLibrary::RandomIntegerInRange(0, Parent->GetDesiredSize().Y)));
	}
}
