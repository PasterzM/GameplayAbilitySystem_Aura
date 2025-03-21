// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraAssetManager& UAuraAssetManager::Get(){
	check(GEngine);
	auto* AuraAssetMAnaget = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetMAnaget;
}

void UAuraAssetManager::StartInitialLoading(){
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();	//potrzebne do uzycia target Data!!
}
