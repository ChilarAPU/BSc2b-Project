// Fill out your copyright notice in the Description page of Project Settings.


#include "MicroscopeBackground.h"

#include "MicroscopeMicrobe.h"
#include "MicroscopeView.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMicroscopeBackground::NativeConstruct()
{
	Super::NativeConstruct();
	bMouseHeld = false;
}

FReply UMicroscopeBackground::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	bMouseHeld = true;
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMicroscopeBackground::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseHeld = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UMicroscopeBackground::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bMouseHeld && IsValid(ParentWidget) && CachedMousePos != UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()))
	{
		for (int i = 0; i < ParentWidget->AllMicrobes.Num(); i++)
		{
			ParentWidget->AllMicrobes[i]->MoveMicrobes(CachedMousePos);
			ParentWidget->AllMicrobes[i]->bFirstRunThrough = false;
		}
		CachedMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UMicroscopeBackground::SetParentWidget(UUserWidget* Parent)
{
	ParentWidget = Cast<UMicroscopeView>(Parent);
}
