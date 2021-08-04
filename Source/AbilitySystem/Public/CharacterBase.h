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

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	bool bIsDead;

		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	uint8 TeamID;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Dead();

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
	void OnHealthChanged(float Health, float MaxHealth);	
	
	UFUNCTION(BlueprintCallable, Category = "CaharacterBase")
	bool IsOtherHostile(ACharacterBase* Other);

	uint8 GetTeamID() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "Die"))
	void BP_Die();
	

	void AutoDeterminateTeamIDByControllerType();
};
