// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter2D.h"

#include "GAS/Attributes/CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

ABaseCharacter2D::ABaseCharacter2D()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	MainAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("MainAttributeSet"));
}

UAbilitySystemComponent* ABaseCharacter2D::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter2D::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();

		GiveDefaultAbilities();
	}
}


void ABaseCharacter2D::InitializeAttributes()
{
	if (!AbilitySystemComponent || DefaultAttributeEffects.IsEmpty())
	{
		return;
	}

	for (auto& SetClass : Attributes)
	{
		if (SetClass)
		{
			UAttributeSet* NewSet = NewObject<UAttributeSet>(this, SetClass);

			AbilitySystemComponent->AddAttributeSetSubobject(NewSet);

			SpawnedAttributeSets.Add(NewSet);
		}
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> EffectClass : DefaultAttributeEffects)
	{
		if (!EffectClass) continue;

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABaseCharacter2D::GiveDefaultAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, -1, this);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}
