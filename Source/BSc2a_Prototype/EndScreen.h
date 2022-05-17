// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreen.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UEndScreen : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* WinScreen;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* LoseScreen;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EndingText;

	UFUNCTION()
	void QuitGame();
};
