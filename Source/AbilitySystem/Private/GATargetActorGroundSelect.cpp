// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorGroundSelect.h"

#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

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

	if (OverlappedActors.Num() > 0)
	{
		const FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);

	DrawDebugSphere(GetWorld(), LookPoint, Radius, 32, FColor::Red, true, -1, 0, 5.0f);
}

bool AGATargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;

	APawn* MasterPawn = MasterPC->GetPawn();

	if (MasterPawn)
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint,
	                                                     ViewPoint + ViewRotation.Vector() * 10000.0f, ECC_Visibility,
	                                                     QueryParams);
	if (TryTrace)
		OutViewPoint = HitResult.ImpactPoint;
	else
		OutViewPoint = FVector();

	return TryTrace;
}
