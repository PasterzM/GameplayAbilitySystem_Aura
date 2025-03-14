// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies(){
	//aktualizowanie parametrów w trakcie rozgrywki, kiedy wartości się zmienią
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data) {
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues(){
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	//uzycie mapy [tag, atrybut] w celu broadcast (uzycia delegata), patrz UAuraAttributeSet::UAuraAttributeSet
	for (auto& Pair : AS->TagsToAttributes) {
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& AttributeValue) const{
	auto info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	info.AttributeValue = AttributeValue.GetNumericValue(AttributeSet);
	AttibuteInfoDelegate.Broadcast(info);
}
