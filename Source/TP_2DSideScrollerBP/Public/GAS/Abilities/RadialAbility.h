// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RadialAbility.generated.h"

/**
 * 
 */
UCLASS()
class TP_2DSIDESCROLLERBP_API URadialAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;	
	
	UPROPERTY(EditDefaultsOnly)
	FVector2D SpawnOffset;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AbilityRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, Category = "Combat")
    bool bShowDebug = true;
#endif

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;
};
