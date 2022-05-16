// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroscopeView.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UMicroscopeView : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* MainView;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBackgroundBlur* Blur;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* BlurSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UMicroscopeMicrobe* Microbe;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUserWidget* BackgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	//This will hold the value at which the microscope is no longer blurry. Will be between 0 - 1
	UPROPERTY()
	float ClearValue;

	//Holds all the spawned in microbes in case I ever need to access them
	UPROPERTY()
	TArray<class UMicroscopeMicrobe*> AllMicrobes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfMicrobesToSpawn;

	//Used to know how many microbes have been removed from the scene
	UPROPERTY()
	int MicrobesRemoved;

	UPROPERTY()
	FVector2D MaxViewportSize;
	
	UFUNCTION()
	void OnBlurValueChange(float InValue);

	UFUNCTION()
	void ExitView();
	
	bool bMouseHeld;
	FVector2D MousePos;

	UFUNCTION()
	void ResetView();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
