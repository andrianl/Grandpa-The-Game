// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "BaseCharacter2D.generated.h"

class UAbilitySystemComponent;
class UCharacterAttributeSet;
class UGameplayAbility;

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
    None,
    Confirm,
    Cancel,
    MeleeAttack,
    RangedAttack,
    Jump
};

UCLASS(config=Game, PrioritizeCategories = "GAS GAS|MainAttributeSet GAS|Attributes GAS|DefaultAttributesEffects GAS|DefaultAbilities")
class TP_2DSIDESCROLLERBP_API ABaseCharacter2D : public APaperCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

    ABaseCharacter2D();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS|MainAttributeSet")
    TObjectPtr<UCharacterAttributeSet> MainAttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes|RuntimeDefaultAttributes")
    TArray<TSubclassOf<UAttributeSet>> Attributes;

    UPROPERTY()
    TArray<TObjectPtr<UAttributeSet>> SpawnedAttributeSets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DefaultAttributesEffects")
    TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffects;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DefaultAbilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:

    void InitializeAttributes();
    void GiveDefaultAbilities();
};
