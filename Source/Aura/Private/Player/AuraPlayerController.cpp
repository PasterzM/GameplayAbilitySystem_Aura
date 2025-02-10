// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"

AAuraPlayerController::AAuraPlayerController(){
	bReplicates = true; //je�li obiekt zmieni stan na serwwerze zostanie to wys�ane klientom.
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime){
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun(){
	if (!bAutoRuning) return;
	if (APawn* ControlledPawn = GetPawn()) {
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius) {
			bAutoRuning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay(){
	Super::BeginPlay();

	check(AuraContext); //co� jak assercja, nie pozwala dzia�a� programowi dalej

	//GetSubsystem -> subsystemy s� singletonami
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//input do widget�w
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //nieblokowanie myszki do viewportu
	InputModeData.SetHideCursorDuringCapture(false); //nei chowac kursora kiedy wejdzie na viewport

	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	//CastChecked po za rzutowaniem jeszcze assercja
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue){
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace(){
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor == nullptr) {
		if (ThisActor != nullptr) {
			ThisActor->HighlightActor();
		} else {}
	} else {
		if (ThisActor == nullptr) {
			LastActor->UnHighlightActor();
		} else {
			if (ThisActor != LastActor) {
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			} else {}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag){
	//	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) {
		bTargeting = ThisActor ? true : false;
		bAutoRuning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag){
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) {
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (bTargeting) {
		if (GetASC()) {
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	} else {
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) {
			if (UNavigationPath* navPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination)) {
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : navPath->PathPoints) {
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				CachedDestination = navPath->PathPoints[navPath->PathPoints.Num()-1];
				bAutoRuning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag){
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) {
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting) {
		if (GetASC()) {
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	} else {
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) {
			CachedDestination = Hit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn()) {
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection); //działa przy multiplayer
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC(){
	if (AuraAbilitySystemComponent == nullptr) {
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}
