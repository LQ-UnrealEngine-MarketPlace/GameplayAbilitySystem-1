// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "AIController.h"
#include "AttributeSetBase.h"
#include "BrainComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSetBaseComponent = CreateDefaultSubobject<UAttributeSetBase>("AttributeSetBaseComponent");
	bIsDead = false;
	TeamID = 255;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AttributeSetBaseComponent->OnHealthChanged.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AutoDeterminateTeamIDByControllerType();
}

void ACharacterBase::Dead()
{
	bIsDead = true;
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}
	else
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->GetBrainComponent()->StopLogic("Death");
		}
	}

	BP_Die();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAcquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire, 1, 0));
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if (!bIsDead && Health <= 0.0f)
	{
		Dead();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	return Other->GetTeamID() != this->GetTeamID();
}

uint8 ACharacterBase::GetTeamID() const
{
	return TeamID;
}

void ACharacterBase::AutoDeterminateTeamIDByControllerType()
{
	if (GetController() && GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
}

