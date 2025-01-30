// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* worldContextObject){
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(worldContextObject, 0)) {
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetContollerParams(PC, PS, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WidgetContollerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* worldContextObject){
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(worldContextObject, 0)) {
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetContollerParams(PC, PS, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetContollerParams);
		}
	}
	return nullptr;
}
