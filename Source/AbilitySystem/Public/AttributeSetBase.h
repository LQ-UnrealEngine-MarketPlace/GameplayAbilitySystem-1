// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, float, Health, float, MaxHealth);

UCLASS()
class ABILITYSYSTEM_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()


public:
	UAttributeSetBase();
	
	UPROPERTY(EditAnywhere, BlueprintCallable, BlueprintAssignable, Category = "AttributeSetBase")
	FHealthChangedDelegate OnHealthChanged;
	
	UPROPERTY(EditAnywhere, BlueprintCallable, BlueprintAssignable, Category = "AttributeSetBase")
	FHealthChangedDelegate OnManaChanged;
	
	UPROPERTY(EditAnywhere, BlueprintCallable, BlueprintAssignable, Category = "AttributeSetBase")
	FHealthChangedDelegate OnStrengthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Mana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Strength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxStrength;

	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
