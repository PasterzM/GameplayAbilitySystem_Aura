// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy: public AAuraCharacterBase, public IEnemyInterface{
	GENERATED_BODY()
	public:
	AAuraEnemy();
	//emeny interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor()override;
	//end enemy interface

	//combat interface
	virtual int32 GetPlayerLevel() override;
	//end combat interface

	protected:
	virtual void BeginPlay() override;
	//UPROPERTY(BlueprintReadOnly)
	//bool bHighlighted = false;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

};
