// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility){
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate(){
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult cursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	ValidData.Broadcast(cursorHit.Location);
}
