// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "AbilityTypes.h"
#include "AIController.h"
#include "AttributeSetBase.h"
#include "BasePlayerController.h"
#include "BrainComponent.h"
#include "GameplayAbilityBase.h"
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
	AttributeSetBaseComponent->OnManaChanged.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComponent->OnStrengthChanged.AddDynamic(this, &ACharacterBase::OnStrengthChanged);
	AutoDeterminateTeamIDByControllerType();
	AddGameplayTag(FullHealthTag);
}

void ACharacterBase::Dead()
{
	bIsDead = true;

	DisableInputControl();

	BP_Die();
}

void ACharacterBase::DisableInputControl() const
{
	if (bIsDead) return;;

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
}

void ACharacterBase::EnableInputControl() const
{
	if (bIsDead) return;;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->EnableInput(PlayerController);
	}
	else
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->GetBrainComponent()->RestartLogic();
		}
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd) const
{
	ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(GetController());
	if (!BasePlayerController) return;

	UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
	if (!AbilityInstance) return;

	const FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
	BasePlayerController->AddAbilityToUI(AbilityInfo);
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

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::OnStrengthChanged(float Strength, float MaxStrength)
{
	BP_OnStrengthChanged(Strength, MaxStrength);
}

void ACharacterBase::HitStun(float StunDuration)
{
	DisableInputControl();
	if (StunTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(StunTimerHandle);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ACharacterBase::EnableInputControl, StunDuration, false);
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	return Other->GetTeamID() != this->GetTeamID();
}

uint8 ACharacterBase::GetTeamID() const
{
	return TeamID;
}

void ACharacterBase::AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire)
{
	for (const auto AbilityItem : AbilitiesToAcquire)
	{
		AcquireAbility(AbilityItem);

		if (!AbilityItem->IsChildOf(UGameplayAbilityBase::StaticClass())) continue;

		TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
		if (AbilityBaseClass == nullptr) continue;
		AddAbilityToUI(AbilityBaseClass);
	}
}

void ACharacterBase::AutoDeterminateTeamIDByControllerType()
{
	if (GetController() && GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
}
