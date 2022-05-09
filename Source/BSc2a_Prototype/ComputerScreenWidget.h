// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComputerScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UComputerScreenWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	//Used to set the cursor image to the position of the cursor but only when the cursor is inside the widget
	virtual FReply NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	//Currently not used
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* Password;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ComputerCursor;

	/* the password the player is trying to match/find scattered around the room
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TargetPassword;

	//Function for when the exit button has been pressed
	UFUNCTION()
	void ExitComputer();

	//Function for when the player presses the enter or return key when typing in the password
	UFUNCTION()
	void PasswordCheck(const FText& InText, ETextCommit::Type InCommit);

	void SetImagePositionToMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	
};
