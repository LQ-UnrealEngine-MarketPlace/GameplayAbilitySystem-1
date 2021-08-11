// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorAround.h"

#include "Abilities/GameplayAbility.h"
#include "Components/DecalComponent.h"

void AGATargetActorAround::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	//Decal->DecalSize = FVector(Radius);
}

void AGATargetActorAround::ConfirmTargetingAndContinue()
{
	APawn* OwningPawn = MasterPC->GetPawn();

	if(!OwningPawn)
		return;
	
	FVector ViewLocation = OwningPawn->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	const bool TraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;

	if (MasterPC->GetPawn())
		CollisionQueryParams.AddIgnoredActor(MasterPC->GetPawn());

	const bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, ViewLocation, FQuat::Identity,
																FCollisionObjectQueryParams(ECC_Pawn),
																FCollisionShape::MakeSphere(Radius), CollisionQueryParams);

	if (TryOverlap)
	{
		for (int32 index = 0; index < Overlaps.Num(); ++index)
		{
			APawn* PawnOverlapped = Cast<APawn>(Overlaps[index].GetActor());
			if (PawnOverlapped && !OverlappedActors.Contains(PawnOverlapped))
			{
				OverlappedActors.Add(PawnOverlapped);
			}
		}
	}

	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}

	if (OverlappedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation));
	}
}
