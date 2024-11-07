// Fill out your copyright notice in the Description page of Project Settings.


#include <GameplayEffectExtension.h>
#include <GameFramework/Character.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet(){
	UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::UAuraAttributeSet"));

	//metoda stworzona przez makro ATTRIBUTE_ACCESSORS
	InitHealth(50.f);
	InitMaxHealth(200.f);
	InitMana(50.f);
	InitMaxMana(200.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//jeszcze REPNOTIFY_OnChanged
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue){
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	//if(Attribute == GetMaxHealthAttribute()){
	//	UE_LOG(LogTemp, Warning, TEXT("MaxHealth %f"), NewValue);
	//}
	if(Attribute == GetManaAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	//if(Attribute == GetMaxManaAttribute()){
	//	UE_LOG(LogTemp, Warning, TEXT("MaxMana %f"), NewValue);
	//}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& props) const{
	//Source = causer of the effect, Target = target of the effect (owner of this AS)
	props.EffectContextHandle = Data.EffectSpec.GetContext();
	props.SourceASC = props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(props.SourceASC) && props.SourceASC->AbilityActorInfo.IsValid() && props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()){
		props.SourceAvatarActor = props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		props.SourceController = props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(props.SourceController == nullptr && props.SourceAvatarActor != nullptr){
			if(const APawn* Pawn = Cast<APawn>(props.SourceAvatarActor)){
				props.SourceController = Pawn->GetController();
			}
		}
		if(props.SourceController){
			ACharacter* SourceCharacter = Cast<ACharacter>(props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()){
		props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		props.TargetCharacter = Cast<ACharacter>(props.TargetAvatarActor);
		props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data){
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties props;
	SetEffectProperties(Data, props);


}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);	//uruchamia elementy pod GAS, istotne przy zmianach
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);	//uruchamia elementy pod GAS, istotne przy zmianach
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);	//uruchamia elementy pod GAS, istotne przy zmianach

}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);	//uruchamia elementy pod GAS, istotne przy zmianach

}