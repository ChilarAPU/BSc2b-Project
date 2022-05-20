// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueWidget.h"
#include "Grabber.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"

#include "CustomPlayer.generated.h"

class UCurveFloat;

UCLASS()
class BSC2A_PROTOTYPE_API ACustomPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PlayerHands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGrabber* Grabber;

	/*This boolean may not be needed visually but stops the function from continually firing off during gameplay,
	wasting resources */
	UPROPERTY()
	bool bRevertTilt;

	/* Used by the camera tilting mechanic to know what direction it needs to tilt to in order to go back into the center
	 */
	UPROPERTY()
	float CachedAxis;

	/* Used alongside the CachedAxis variable to know what roll value the camera needs to be at in order to be considered
	 * straight. This value is usually always 0 but using this variable is safer.
	 */
	UPROPERTY()
	float StartingRollValue;

	UPROPERTY()
	bool bInRangeOfInteractable;

	UPROPERTY()
	bool bRotateObject;

	UPROPERTY()
	bool bFirstPlayThrough;
	
	//Tilting Timeline specific
	UFUNCTION()
	void TimelineProgress(float Value);

	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FRotator StartRot;
	UPROPERTY()
	FRotator EndRot;
	UPROPERTY(EditAnywhere, Category = "Timeline");
	float RollTiltOffset;
	//end of timeline functionality

	//Was going to combine both timeline functions into one but theres so many changes that would have to be made,
	//that it is not really viable or faster than just creating new ones.
	
	//Hand Animation Timeline specific
	UFUNCTION()
	void HandTimelineProgress(float Value);

	FTimeline HandTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* HandFloat;

	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ZOffset;
	//End of hand animation functionality
	

	/* Forces the timeline to only run once when this variable is false. Could stick it into BeginPlay but that would
	 * complicate the process of stopping it then replaying it.
	 */
	UPROPERTY()
	bool bHandAnimPlaying;
	

	UPROPERTY()
	int NumberOfTasksCompleted;

	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	class USoundCue* FootstepSound;

	/* Used alongside HandTimelineProgress() to make sure that the footstep sound only plays once
	 */
	bool bPlayedFootstep;

	/* Used to access the widget blueprint. Without this, would only be able to access the C++ version
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDialogueWidget> DialogueUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainMenu> MainMenu;

	bool MainMenuOpen;

	UPROPERTY()
	UMainMenu* MainMenuRef;
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Player Input Functions
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void MouseMoveX(float Axis);
	void MouseMoveY(float Axis);

	UFUNCTION()
	void MainMenuPressed();

	void RotateObject();

	void Interact();

	void SpawnDialogueUI(APlayerController* PlayerControl);

	/* function that encapsulates the line trace code to make the main function easier to read
	 */
	void LineTraceFromCamera(FHitResult& OutHit);

	/*Used to handle the left mouse button. Has different uses depending on whether the player is currently
	 *playing a minigame or not
	 */
	void Grab();

	void Release()
	{
		Grabber->Release();
	}

	/*
	 *Handles tilting the camera during movement along with reverting this tilt. Includes variable called TiltAmount that
	 *allows easy editing of the degree of tilt
	 */
	void MoveCameraTilt(float Axis);

	/* Setup for the hand animation timeline. Very similar to MoveCameraTilt but with enough differences to allow a new
	 * function
	 */
	void HandAnimationSetup(float Axis);

	/* Used with MoveCameraTilt and handles what the timeline should do during playback
	 */
	void StartTimeline(float Axis, float TiltAmount);

	/* Used with HandAnimationSetup and handles what the timeline should do during playback
	 */
	void StartHandTimeline(float Axis, float ZAmount);

	void PlayFootstep();

	/* Will be called by interactables overlap functions to start a line trace. This will force the player to look at the
	 * object when they want to interact
	 */
	void InRangeOfInteractable();
	
	/*
	 *This allows use of the functionality of bUsePawnControlRotation but leaving out one of the rotation parameters (Roll)
	 *to enable some custom rotation behaviour on that axis.
	 **/
	void CustomCameraUsePawnRotation()
	{
		Camera->SetWorldRotation(FRotator(GetViewRotation().Pitch, GetViewRotation().Yaw, Camera->GetComponentRotation().Roll));
	}

	//Encapsulates the functionality that changes from one camera to the other. Gets used quite a lot during this project
	void ChangeViewTarget(class AActor* IncomingClass, APlayerController* PlayerC);
};
