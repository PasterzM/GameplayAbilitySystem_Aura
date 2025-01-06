// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


void AAuraPlayerState::BeginPlay(){
	UE_LOG(LogTemp, Warning, TEXT("AAuraPlayerState::BeginPlay"));
}

AAuraPlayerState::AAuraPlayerState(){
	UE_LOG(LogTemp, Warning, TEXT("AAuraPlayerState::AAuraPlayerState"));

	NetUpdateFrequency = 100.f; //jak czesto serwer bedzie probowac updatowac klientów

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const{
	return AttributeSet;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel){
}
