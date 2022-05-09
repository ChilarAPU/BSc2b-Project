// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroscopeMicrobe.generated.h"

/**
 * 
 */
UCLASS()
class BSC2A_PROTOTYPE_API UMicroscopeMicrobe : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Microbe;

	bool bIsBad;

	FVector2D MaxSize;

	int MicrobeDirection;
	float AmountToMoveX;
	float AmountToMoveY;
	float MoveIncrement;
	float ChangeDirectionSpeed;

	bool bFirstRunThrough;

	//Holds the parent widget. This case is MicroscopeView
	UPROPERTY()
	class UMicroscopeView* Parent;

	UFUNCTION()
	void MicrobePress(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	void IsMicrobeBad(class UMicroscopeView* OwningWidget);

	UFUNCTION()
	void MoveMicrobes(FVector2D MousePosition);

	UFUNCTION()
	void SetRandomSpawnPoint();
};
