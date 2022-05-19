// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayer.h"

#include "BaseWeighingObject.h"
#include "BSc2a_PrototypeGameModeBase.h"
#include "ComputerMinigame.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "EndGoal.h"
#include "Grabber.h"
#include "MainMenu.h"
#include "MicroscopeMinigame.h"
#include "MicroscopeView.h"
#include "WeighingMinigame.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
ACustomPlayer::ACustomPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());

	PlayerHands = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerHands"));
	PlayerHands->SetupAttachment(Camera);

	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));
	Grabber->SetupAttachment(Camera);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	

	//Do not want a floating pair of hands casting a shadow
	PlayerHands->SetCastShadow(false);

	//Var Defaults
	bRevertTilt = false;
	CachedAxis = 0;
	StartingRollValue = 0;
	bHandAnimPlaying = false;
	bPlayedFootstep = false;
	bInRangeOfInteractable = false;
	NumberOfTasksCompleted = 0;
	bRotateObject = false;
	GetCharacterMovement()->MaxWalkSpeed = 500;
	MainMenuOpen = false;
	GetCharacterMovement()->MaxStepHeight = 100;
}

void ACustomPlayer::TimelineProgress(float Value)
{
	FRotator NewRot = FMath::Lerp(StartRot, EndRot, Value);
	Camera->SetRelativeRotation(FRotator(Camera->GetRelativeRotation().Pitch, Camera->GetRelativeRotation().Yaw, NewRot.Roll));
}

void ACustomPlayer::HandTimelineProgress(float Value)
{
	//A temporary fix but it stops the hands from moving offscreen when the player moves up or down on the ZAxis.
	//Does make the hands higher up than I originally wanted them to but is something I can live with
	EndLoc.Z = (Camera->GetComponentLocation().Z - 15) + ZOffset;
	StartLoc.Z = Camera->GetComponentLocation().Z - 15;
	FVector NewLoc = FMath::Lerp(StartLoc, EndLoc, Value);
	PlayerHands->SetWorldLocation(FVector(PlayerHands->GetComponentLocation().X, PlayerHands->GetComponentLocation().Y, NewLoc.Z));
	//Match up footsteps with the bobbing animation. -1 equals the lowest point of the bob
	

	bool bAtLowestPointOfAnim = FMath::IsNearlyEqual(Value, -1.f, .1f);
	if (!bPlayedFootstep && bAtLowestPointOfAnim)
	{
		PlayFootstep();
		bPlayedFootstep = true;
	} else if (!bAtLowestPointOfAnim)
	{
		bPlayedFootstep = false;
	}
	
}

// Called when the game starts or when spawned
void ACustomPlayer::BeginPlay()
{
	Super::BeginPlay();

	StartingRollValue = Camera->GetComponentRotation().Roll;
	
}

// Called every frame
void ACustomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CustomCameraUsePawnRotation();

	CurveTimeline.TickTimeline(DeltaTime);
	HandTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ACustomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveMouseX", this, &ACustomPlayer::MouseMoveX);
	PlayerInputComponent->BindAxis("MoveMouseY", this, &ACustomPlayer::MouseMoveY);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACustomPlayer::Interact);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ACustomPlayer::Grab);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &ACustomPlayer::Release);
	PlayerInputComponent->BindAction("RotateObject", IE_Pressed, this, &ACustomPlayer::RotateObject);
	PlayerInputComponent->BindAction("RotateObject", IE_Released, this, &ACustomPlayer::RotateObject);
	FInputActionBinding& Toggle =  PlayerInputComponent->BindAction("MainMenu", IE_Pressed, this, &ACustomPlayer::MainMenuPressed);
	Toggle.bExecuteWhenPaused = true;
}

void ACustomPlayer::MoveForward(float Axis)
{
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), Axis);
		
		HandAnimationSetup(Axis);
	}
}

void ACustomPlayer::MoveRight(float Axis)
{
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorRightVector(), Axis);
		
		ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode->bTiltCameraWhenMoving)
		{
			MoveCameraTilt(Axis);
		}
		
	}
}

void ACustomPlayer::MouseMoveX(float Axis)
{
	if (!bRotateObject)
	{
		AddControllerYawInput(Axis);
	} else
	{
		//Rotate whatever object is currently being help by the physics grabber
		//Currently not 100 percent functional
		FVector Loc;
		FRotator Rot;
		PhysicsHandle->GetTargetLocationAndRotation(Loc, Rot);
		FRotator NewRot = UKismetMathLibrary::MakeRotator(0, 0, Rot.Yaw * Axis);
		FRotator tempRot = UKismetMathLibrary::ComposeRotators(Rot, NewRot);
		PhysicsHandle->SetTargetRotation(tempRot);
		
	}
	
}

void ACustomPlayer::MouseMoveY(float Axis)
{
	if (!bRotateObject)
	{
		AddControllerPitchInput(Axis);
	} else
	{
		//Rotate whatever object is currently being help by the physics grabber
		//Currently not 100 percent functional
		FVector Loc;
		FRotator Rot;
		PhysicsHandle->GetTargetLocationAndRotation(Loc, Rot);
		FRotator NewRot = UKismetMathLibrary::MakeRotator(0, (Rot.Pitch * -Axis) * 5, 0);
		FRotator tempRot = UKismetMathLibrary::ComposeRotators(Rot, NewRot);
		PhysicsHandle->SetTargetRotation(tempRot);
	}
	
}

void ACustomPlayer::MainMenuPressed()
{
	if (MainMenu)
	{
		APlayerController* PlayerC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (MainMenuOpen)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			MainMenuRef->RemoveFromParent();
			PlayerC->SetIgnoreMoveInput(false);
			FInputModeGameOnly GameMode;
			PlayerC->SetShowMouseCursor(false);
			PlayerC->SetInputMode(GameMode);
			MainMenuOpen = false;
		} else
		{
			MainMenuRef = Cast<UMainMenu>(CreateWidget(GetWorld(), MainMenu));
			FString Text = "PAUSED";
			MainMenuRef->Title->SetText(FText::FromString(Text));
			MainMenuRef->AddToViewport();
			PlayerC->SetIgnoreMoveInput(true);
			FInputModeGameAndUI GameAndUIMode;
			PlayerC->SetShowMouseCursor(true);
			PlayerC->SetInputMode(GameAndUIMode);
			MainMenuOpen = true;
		}
		
	}
}

void ACustomPlayer::RotateObject()
{
	if (bRotateObject)
	{
		bRotateObject = false;
	} else
	{
		bRotateObject = true;
	}
}


void ACustomPlayer::Interact()
{
	if (bInRangeOfInteractable)
	{
		APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
		FHitResult OutHit;
		LineTraceFromCamera(OutHit);
		//PlayerHands->SetVisibility(false);
		
		if (Cast<AInteractableBaseClass>(OutHit.GetActor()))
		{
			ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			//Code relevant to all interactions goes here
			
			if (Cast<AEndGoal>(OutHit.GetActor()))
			{
				GetCharacterMovement()->Velocity = FVector();
				PlayerC->SetIgnoreMoveInput(true);
				FInputModeUIOnly DialougeOpen;
				DialougeOpen.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerC->SetShowMouseCursor(true);
				PlayerC->SetInputMode(DialougeOpen);
				SpawnDialogueUI(PlayerC);
			}
			else if (AComputerMinigame* Computer = Cast<AComputerMinigame>(OutHit.GetActor()))
			{
				GetCharacterMovement()->Velocity = FVector();
				PlayerC->SetIgnoreMoveInput(true);
				FInputModeUIOnly DialougeOpen;
				DialougeOpen.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerC->SetShowMouseCursor(true);
				PlayerC->SetInputMode(DialougeOpen);
				ChangeViewTarget(Computer, PlayerC);
			}
			else if (AWeighingMinigame* Weigh = Cast<AWeighingMinigame>(OutHit.GetActor()))
			{
				ChangeViewTarget(Weigh, PlayerC);
				//Change input mode for only this interaction as it needs the mouse cursor events
				GetCharacterMovement()->Velocity = FVector();
				PlayerC->SetIgnoreMoveInput(true);
				PlayerC->SetShowMouseCursor(true);
				FInputModeGameAndUI AllowCursor;
				PlayerC->SetInputMode(AllowCursor);
			}
			else if (AMicroscopeMinigame* Micro = Cast<AMicroscopeMinigame>(OutHit.GetActor()))
			{
				if (!GameMode->bHasPlayerBeatMicroscope)
				{
					GetCharacterMovement()->Velocity = FVector();
					PlayerC->SetIgnoreMoveInput(true);
					FInputModeUIOnly DialougeOpen;
					DialougeOpen.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
					PlayerC->SetShowMouseCursor(true);
					PlayerC->SetInputMode(DialougeOpen);
					ChangeViewTarget(Micro, PlayerC);
					//Wait for the amount of time the camera blend takes to finish, then run the function
					Micro->TimerDel.BindUFunction(Micro, FName("SpawnWidget"), PlayerC);
					GetWorld()->GetTimerManager().SetTimer(Micro->TimerHandle, Micro->TimerDel, .5f, false);
				}
				
				
			}
		}
	}
}

void ACustomPlayer::SpawnDialogueUI(APlayerController* PlayerControl)
{
	if (IsValid(DialogueUI))
	{

		UDialogueWidget* LocalDialogueUI = CreateWidget<UDialogueWidget>(PlayerControl, DialogueUI);
		LocalDialogueUI->AddToViewport();
		ABSc2a_PrototypeGameModeBase* GameMode = Cast<ABSc2a_PrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode->AmountOfMinigamesCompleted == 5)
		{
			LocalDialogueUI->bALlowWin = true;
		}

			
		//one is to end the game now
		//other will be locked until tasks are complete
	}
}

void ACustomPlayer::LineTraceFromCamera(FHitResult& OutHit)
{
	//Spawn line trace from the middle of the camera
	float LineTraceLength = 200;
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + Camera->GetForwardVector() * LineTraceLength;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	
	//Remove during building as its only for debugging
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.f);
}

void ACustomPlayer::Grab()
{
	APlayerController* PlayerC = UGameplayStatics::GetPlayerController(this, 0);
	FHitResult OutHit;
	//If we are playing a mini-game, then we cant move the player
	if (PlayerC->IsMoveInputIgnored())
	{
		//This hit result ensures that we cant accidentally click on a random object
		PlayerC->GetHitResultUnderCursor(ECC_Visibility, true, OutHit);
		if (ABaseWeighingObject* Obj = Cast<ABaseWeighingObject>(OutHit.GetActor()))
		{
			if (!Obj->bCanBePickedUp)
			{
			if (Obj->bBeingClicked)
				{
				//reset it when we place the object down
				Obj->bBeingClicked = false;
				} else
				{
					//Bring the object closer to the camera to visibly show that the item has been picked up
					Obj->SetActorLocation(FVector(OutHit.Location.X, OutHit.Location.Y, Obj->GetActorLocation().Z + 12));
					Obj->bBeingClicked = true;
					Obj->bHitObject = false;
				}	
			}
			
			
		} else if (AWeighingMinigame* te = Cast<AWeighingMinigame>(OutHit.GetActor()))
		{
			//If we clicked on the reset weight static mesh
			if (OutHit.GetComponent() == te->ResetScale)
			{
				if (te->TotalWeight == 0)
				{
					te->ResetCamera(this);
				} else
				{
					//Reset weight to 0 and stop and timelines that are currently running
					te->ScaleAmount->SetText("0");
					te->TotalWeight = 0;
					te->TextTimeline.Stop();
				}
				
			}
		}
	} else
	{
		//Grab an object with the physics grabber
		Grabber->Grab();
	}
	
}

void ACustomPlayer::MoveCameraTilt(float Axis)
{
	if (Axis != 0)
	{
		StartTimeline(Axis, RollTiltOffset);
		bRevertTilt = true;
		CachedAxis = Axis;
	} else if (Axis == 0 && bRevertTilt)
	{
		StartTimeline(Axis, RollTiltOffset);
		bRevertTilt = false;
	}
}

void ACustomPlayer::HandAnimationSetup(float Axis)
{
	//Only want the animation to play once, then continue on forever
	if (!bHandAnimPlaying)
	{
		StartHandTimeline(Axis, ZOffset);
		bHandAnimPlaying = true;
	}
	//Set the playback rate to be faster during movement
	if (Axis != 0)
	{
		HandTimeline.SetPlayRate(1.2);
	} else if (Axis == 0)
	{
		HandTimeline.SetPlayRate(.3);
	}
}

void ACustomPlayer::StartTimeline(float Axis, float TiltAmount)
{
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);

		StartRot = EndRot = Camera->GetRelativeRotation();
		
		if (Axis != 0) //Tilting the camera
		{
			EndRot.Roll += TiltAmount * Axis;
		} else //Reverting the tilt
		{
			EndRot.Roll = StartingRollValue;
		}
		EndRot.Roll = FMath::Clamp(EndRot.Roll, -TiltAmount, TiltAmount);
		
		CurveTimeline.PlayFromStart();
	}
}

void ACustomPlayer::StartHandTimeline(float Axis, float ZAmount)
{
	if (HandFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("HandTimelineProgress"));
		HandTimeline.AddInterpFloat(HandFloat, TimelineProgress);
		HandTimeline.SetLooping(true);
		StartLoc = EndLoc = PlayerHands->GetComponentLocation();
		EndLoc.Z += ZAmount;
		HandTimeline.PlayFromStart();
	}
}

void ACustomPlayer::PlayFootstep()
{
	//Do not want to play footstep sounds if the player is not moving
	bool bIsMoving = GetVelocity().Size() != 0;
	if (FootstepSound && bIsMoving)
	{
		//Give a slight change to each footstep sound. Not too much to allow this sound to stay in the background
		FootstepSound->PitchMultiplier = FMath::FRandRange(.7f, 1.3f);
		UGameplayStatics::PlaySound2D(this, FootstepSound);
	}
}

void ACustomPlayer::InRangeOfInteractable()
{
	if (bInRangeOfInteractable)
	{
		bInRangeOfInteractable = false;
	} else
	{
		bInRangeOfInteractable = true;
	}
}

void ACustomPlayer::ChangeViewTarget(AActor* IncomingClass, APlayerController* PlayerC)
{
	FViewTargetTransitionParams Params;
	Params.BlendTime = .5f;
	Params.BlendFunction = VTBlend_Cubic;
	PlayerC->SetViewTarget(IncomingClass, Params);
}



