// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations(){
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions; //dzielenie powierchni na ktorej ma summoning zrobic na sekcje

	//obrót o 45 (połowa 90) używając Z vector 
	const FVector leftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++) {
		const FVector Direction = leftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(minSpawnDistance, maxSpawnDistance);

		FHitResult Hit;
		//sprawdzanie czy są na ziemi
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit) {
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);

		//DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * maxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * minSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
		//DrawDebugSphere(GetWorld(), Location + Direction * maxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass(){
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
