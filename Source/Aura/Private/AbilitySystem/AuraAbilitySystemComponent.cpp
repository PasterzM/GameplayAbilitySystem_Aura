// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilitytActorInfoSet(){
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities){
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities) {
		//const
		auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		//GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);// tutaj AbilitySpec bez const 
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec,
                                                FActiveGameplayEffectHandle ActiveGameplayEffectHandle){
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
