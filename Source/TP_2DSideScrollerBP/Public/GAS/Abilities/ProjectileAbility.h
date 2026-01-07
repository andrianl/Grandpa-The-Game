// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Actors/BaseProjectile2D.h"
#include "ProjectileAbility.generated.h"

UCLASS()
class TP_2DSIDESCROLLERBP_API UProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<ABaseProjectile2D> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float SpawnOffset = 50.0f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;
};
