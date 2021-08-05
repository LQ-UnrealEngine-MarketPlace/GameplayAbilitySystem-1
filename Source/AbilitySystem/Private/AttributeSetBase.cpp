// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

UAttributeSetBase::UAttributeSetBase()
: Health(200.0f), MaxHealth(200.0f), Mana(100.0f), MaxMana(150.0f), Strength(250.0f), MaxStrength(250.0f)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetBaseValue()));
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.0f, MaxMana.GetBaseValue()));
		OnManaChanged.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strength)))
	{
		Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.0f, MaxStrength.GetCurrentValue()));
		Strength.SetBaseValue(FMath::Clamp(Strength.GetBaseValue(), 0.0f, MaxStrength.GetBaseValue()));
		OnStrengthChanged.Broadcast(Strength.GetCurrentValue(), MaxStrength.GetCurrentValue());
	}
}
