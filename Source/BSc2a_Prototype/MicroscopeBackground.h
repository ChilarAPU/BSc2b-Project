// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroscopeBackground.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UMicroscopeBackground : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;

	UPROPERTY()
	class UMicroscopeView* ParentWidget;

	bool bMouseHeld;

	FVector2D MousePos;
	FVector2D CachedMousePos;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//Allows me to access the parent widget. This gets called in the parents constructor
	UFUNCTION()
	void SetParentWidget(UUserWidget* Parent);
	
};
