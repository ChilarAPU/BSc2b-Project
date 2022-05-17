// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class BSC2A_PROTOTYPE_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* WinButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Escape;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* EndGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DirectPlayerText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UEndScreen> EndScreen;

	UPROPERTY()
	UEndScreen* EndScreenRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundWave* ButtonClickSound;

	UPROPERTY()
	bool bALlowWin;

	bool bFinishEarly;

	//Text animation specific variables
	FTimerHandle TextAnim;
	FString InputText;
	int StringIndex;
	FString DisplayedText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TextSpeed;

	UFUNCTION()
	void EscapeClicked();

	UFUNCTION()
	void EndClicked();

	UFUNCTION()
	void WinClicked();

	/* Puts incoming Text into an array that separates each letter from each other and will slowly add it into a
	 * visible string that is shown in the dialogue box.
	 */
	UFUNCTION()
	void DisplayText();
	
};
