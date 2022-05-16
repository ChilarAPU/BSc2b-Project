// Fill out your copyright notice in the Description page of Project Settings.


#include "MicroscopeMicrobe.h"

#include "BSc2a_PrototypeGameModeBase.h"
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
	Microbe2->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");
	Microbe3->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");
	Microbe4->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");
	Microbe5->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");
	Microbe6->OnMouseButtonDownEvent.BindUFunction(this, "MicrobePress");

	ABSc2a_PrototypeGameModeBase* GM = Cast<ABSc2a_PrototypeGameModeBase>( UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		switch(GM->MicrobeValue)
		{
		case 0: Microbe->SetVisibility(ESlateVisibility::Visible);
			GM->MicrobeValue ++;
			break;
		case 1: Microbe2->SetVisibility(ESlateVisibility::Visible);
			Microbe->SetVisibility(ESlateVisibility::Hidden);
			GM->MicrobeValue ++;
			break;
		case 2: Microbe3->SetVisibility(ESlateVisibility::Visible);
			Microbe->SetVisibility(ESlateVisibility::Hidden);
			GM->MicrobeValue ++;
			break;
		case 3: Microbe4->SetVisibility(ESlateVisibility::Visible);
			Microbe->SetVisibility(ESlateVisibility::Hidden);
			GM->MicrobeValue ++;
			break;
		case 4: Microbe5->SetVisibility(ESlateVisibility::Visible);
			Microbe->SetVisibility(ESlateVisibility::Hidden);
			GM->MicrobeValue ++;
			break;
		case 5: Microbe6->SetVisibility(ESlateVisibility::Visible);
			Microbe->SetVisibility(ESlateVisibility::Hidden);
			GM->MicrobeValue ++;
			break;
		case 6: GM->MicrobeValue = 0;
		default: break;
		}
	}
	
	
	
}

void UMicroscopeMicrobe::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UCanvasPanelSlot* CanvasPanel  = UWidgetLayoutLibrary::SlotAsCanvasSlot(GetVisibleMicrobe());;
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
		GetVisibleMicrobe()->SetOpacity(0);
		Parent->MicrobesRemoved++;
		if (Parent->MicrobesRemoved == 1)
		{
			Parent->ResetView();
		}
	}
}

UImage* UMicroscopeMicrobe::GetVisibleMicrobe()
{
	if (Microbe->IsVisible())
	{
		return Microbe;
	} else if (Microbe2->IsVisible())
	{
		return Microbe2;
	} else if (Microbe3->IsVisible())
	{
		return Microbe3;
	} else if (Microbe4->IsVisible())
	{
		return Microbe4;
	} else if (Microbe5->IsVisible())
	{
		return Microbe5;
	} else if (Microbe6->IsVisible())
	{
		return Microbe6;
	} else
	{
		return nullptr;
	}
}

void UMicroscopeMicrobe::IsMicrobeBad(UMicroscopeView* OwningWidget)
{
	if (bIsBad)
	{
		GetVisibleMicrobe()->SetColorAndOpacity(FLinearColor::Red);
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
		UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(GetVisibleMicrobe());
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
		UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(GetVisibleMicrobe());
		CanvasPanel->SetPosition(FVector2D(UKismetMathLibrary::RandomIntegerInRange(0, Parent->GetDesiredSize().X), 
		UKismetMathLibrary::RandomIntegerInRange(0, Parent->GetDesiredSize().Y)));
	}
}
