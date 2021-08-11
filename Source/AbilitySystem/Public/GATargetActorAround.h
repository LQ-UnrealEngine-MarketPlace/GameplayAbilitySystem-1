// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorAround.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorAround : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "FindAround")
	float Radius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "FindAround")
	UDecalComponent* Decal;

	USceneComponent* RootComponent;
	
protected:

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
};
