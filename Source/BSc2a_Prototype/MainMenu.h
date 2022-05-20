// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* BobbingCheck;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* TutorialImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* ButtonClickSound;

	UFUNCTION()
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void TempStartFunction();

	UFUNCTION(BlueprintCallable)
	void StartGameContinued();

	UFUNCTION()
	void SettingsPage();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void BobbingCheckChange(bool Incoming);
};
