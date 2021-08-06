// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAttributeSetBase* AttributeSetBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	FGameplayTag FullHealthTag;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	bool bIsDead;

		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	uint8 TeamID;
	FTimerHandle StunTimerHandle;
public:
	
	// Sets default values for this character's properties
	ACharacterBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire);	
	
	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	bool IsOtherHostile(ACharacterBase* Other);

	uint8 GetTeamID() const;

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void OnHealthChanged(float Health, float MaxHealth);	

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void OnManaChanged(float Mana, float MaxMana);		

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void AddGameplayTag(FGameplayTag& TagToAdd);			

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void RemoveGameplayTag(FGameplayTag& TagToRemove);	

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void OnStrengthChanged(float Strength, float MaxStrength);	

	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	void HitStun(float StunDuration);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStrengthChanged"))
	void BP_OnStrengthChanged(float Strength, float MaxStrength);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "Die"))
	void BP_Die();
	

	void AutoDeterminateTeamIDByControllerType();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Dead();
	
	void DisableInputControl() const;
	void EnableInputControl() const;

};
