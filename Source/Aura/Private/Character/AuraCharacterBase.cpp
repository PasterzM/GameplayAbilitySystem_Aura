// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase(){
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay(){
	Super::BeginPlay();

}

void AAuraCharacterBase::InitAbilityActorInfo(){
}

//Powinno by� od strony serwera, potem rozes�ane do klient�w.
void AAuraCharacterBase::InitializePrimaryAttributes() const{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	const auto ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const auto SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
