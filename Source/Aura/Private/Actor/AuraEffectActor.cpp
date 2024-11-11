// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass){
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle AcctiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool iIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(iIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap){
		ActiveEffectHandles.Add(AcctiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* targetActor){
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(targetActor, InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(targetActor, DurationGameplayEffectClass);
	}
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(targetActor, InstantGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(targetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* targetActor){
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndEverlap){
		ApplyEffectToTarget(targetActor, InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndEverlap){
		ApplyEffectToTarget(targetActor, DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndEverlap){
		ApplyEffectToTarget(targetActor, InfiniteGameplayEffectClass);
	}

	//ActiveEffectHandles
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap){
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(targetActor);
		if(!IsValid(TargetASC)){ return; }
		TArray<FActiveGameplayEffectHandle> toRemove;
		for(auto handlePair : ActiveEffectHandles){
			if(handlePair.Value == TargetASC){
				TargetASC->RemoveActiveGameplayEffect(handlePair.Key, 1);
				toRemove.Add(handlePair.Key);
			}
		}
		for(auto& key : toRemove){
			ActiveEffectHandles.FindAndRemoveChecked(key);
		}
	}
}

void AAuraEffectActor::BeginPlay(){
	Super::BeginPlay();
}