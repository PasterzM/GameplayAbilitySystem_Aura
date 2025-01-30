// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag AttributeTag, bool bLogNotFound) const{
	for (const FAuraAttributeInfo& info : AttributeInformation) {
		if (info.AttributeTag == AttributeTag) {
			return info;
		}
	}
	if (bLogNotFound) {
		UE_LOG(LogTemp,Error,TEXT("Can't find info for AttributeTag [%s] o AttributeInfo [%s]."),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
