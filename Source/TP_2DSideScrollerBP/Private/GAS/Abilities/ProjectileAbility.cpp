// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/ProjectileAbility.h"
#include "GameFramework/Character.h"
#include "Actors/BaseProjectile2D.h"

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // 1. Комміт (перевірка кулдауну/мани)
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 2. Логіка спавну
    const AActor* const Avatar = ActorInfo->AvatarActor.Get();

    FActorSpawnParameters SpawnParams;
    // Важливо: кажемо снаряду ігнорувати колізію того, хто його створив, якщо колізія налаштована правильно
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    if (Avatar && ProjectileClass)
    {
        const FVector SpawnLoc = Avatar->GetActorLocation() + (Avatar->GetActorForwardVector() * SpawnOffset);
        const FRotator SpawnRot = Avatar->GetActorRotation();

        ABaseProjectile2D* const Projectile = 
            GetWorld()->SpawnActor<ABaseProjectile2D>(ProjectileClass, SpawnLoc, SpawnRot, SpawnParams);

        if (IsValid(Projectile))
        {
			UE_LOG(LogTemp, Log, TEXT("Projectile spawned at location: %s"), *SpawnLoc.ToString());

			Projectile->FireInDirection(Avatar->GetActorForwardVector());
		}
    }

    // 3. Завершення
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
