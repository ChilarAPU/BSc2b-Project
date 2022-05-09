// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBaseClass.generated.h"

UCLASS()
class BSC2A_PROTOTYPE_API AInteractableBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBaseClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollideTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextRenderComponent* InteractText;

	//Value that gets shown when a player walks up to a mini-game
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString InteractString;

	//TextAnimation specific
	FTimerHandle TextAnim;
	FTimerHandle ReverseTextAnim;
	int StringIndex;
	FString DisplayedText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Both overlap functions are going to contain the same code, but the delegates cannot link to the same function due
	 * to a difference in parameters
	 */
	void OverlapFunction(AActor* OtherActor);

	/* Called through a timer delegate and handles the letter by letter typing effect for text
	 */
	UFUNCTION()
	void DisplayText();

	/* Resets the text that was created through DisplayText()
	 */
	UFUNCTION()
	void ResetText();

	/* Called by OnOverlapBegin and allows for child classes to have specific functionality only relevant to the child
	 */
	virtual void CustomOverlapBehaviour(AActor* OtherActor);

	/* Same idea as CustomOverlapBehaviour() except this time it is for OnOverlapEnd
	 */
	virtual void CustomOverlapEndBehaviour(AActor* OtherActor);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
