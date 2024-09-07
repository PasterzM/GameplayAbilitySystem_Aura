// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController(){
	bReplicates = true;	//je�li obiekt zmieni stan na serwwerze zostanie to wys�ane klientom.
}

void AAuraPlayerController::BeginPlay(){
	Super::BeginPlay();
	
	check(AuraContext);	//co� jak assercja, nie pozwala dzia�a� programowi dalej

	//GetSubsystem -> subsystemy s� singletonami
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//input do widget�w
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	//nieblokowanie myszki do viewportu
	InputModeData.SetHideCursorDuringCapture(false);	//nei chowac kursora kiedy wejdzie na viewport

	SetInputMode(InputModeData);
}
