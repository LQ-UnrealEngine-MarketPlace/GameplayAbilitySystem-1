// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorGroundSelect.h"

#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"
#include "Components/DecalComponent.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	SetRootComponent(RootComponent);
	Decal->SetupAttachment(RootComponent);

	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
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

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);

	Decal->SetWorldLocation(LookPoint);
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
