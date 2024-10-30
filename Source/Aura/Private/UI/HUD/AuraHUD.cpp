// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams){
	if(!OverlayWidgetController){
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS){

	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized!!, in AAuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized!!, in AAuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	FWidgetControllerParams widgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* widgetController = GetOverlayWidgetController(widgetControllerParams);

	OverlayWidget->SetWidgetController(widgetController);
	widgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}