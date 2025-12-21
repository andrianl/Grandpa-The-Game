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

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FVector FacingDirection2D;

	UFUNCTION(BlueprintCallable, Category = "Combat")
    void UpdateFacingDirection(float InputValue);

	UFUNCTION(BlueprintPure, Category = "Combat")
	FVector GetFacingDirection2D() const { return FacingDirection2D; }

public:
    // --- GAS Компоненти ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS|MainAttributeSet")
    TObjectPtr<UCharacterAttributeSet> MainAttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes|RuntimeDefaultAttributes")
    TArray<TSubclassOf<UAttributeSet>> Attributes;

    UPROPERTY()
    TArray<TObjectPtr<UAttributeSet>> SpawnedAttributeSets;

        // --- Налаштування для Блюпринтів ---

    // 1. Ефекти, які накладаються при старті (встановлюють HP, Mana)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DefaultAttributesEffects")
    TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffects;

    // 2. Абілки, які видаються при старті (Стрільба, Стрибок)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DefaultAbilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:

    // --- Функції Ініціалізації ---
    void InitializeAttributes();
    void GiveDefaultAbilities();
};
