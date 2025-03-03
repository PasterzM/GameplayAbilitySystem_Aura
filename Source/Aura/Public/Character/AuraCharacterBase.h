// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{ return AttributeSet; }
	//Combat interfaces
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override; //Smierć tylko na serwerze
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAtackMontages_Implementation() override;
	//End Combat interfaces
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(); //potrzebne do uruchomienia u wszystkich klientów

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	bool bDead = false;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	//UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> gameplayEffectClass, float level) const;

	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	//Dissolve Effects

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
