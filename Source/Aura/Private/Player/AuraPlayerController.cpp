// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "Interaction/EnemyInterface.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController(){
	bReplicates = true;	//jeœli obiekt zmieni stan na serwwerze zostanie to wys³ane klientom.
}

void AAuraPlayerController::PlayerTick(float DeltaTime){
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay(){
	Super::BeginPlay();

	check(AuraContext);	//coœ jak assercja, nie pozwala dzia³aæ programowi dalej

	//GetSubsystem -> subsystemy s¹ singletonami
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//input do widgetów
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	//nieblokowanie myszki do viewportu
	InputModeData.SetHideCursorDuringCapture(false);	//nei chowac kursora kiedy wejdzie na viewport

	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	//CastChecked po za rzutowaniem jeszcze assercja
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue){
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>()){
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace(){
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit)return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if(LastActor == nullptr){ 
		if(ThisActor != nullptr){ 
			ThisActor->HighlightActor();
		}else{
		
		}
	}else{
		if(ThisActor == nullptr){ 
			LastActor->UnHighlightActor();
		}else{
			if(ThisActor != LastActor){
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}else{
			
			}
		}
	}
}
