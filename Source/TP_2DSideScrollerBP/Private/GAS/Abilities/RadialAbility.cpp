#include "GAS/Abilities/RadialAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void URadialAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // 1. Commit Ability (Check Cooldowns and Costs)
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor* const Avatar = ActorInfo->AvatarActor.Get();
    UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();

    // Basic validity check for source
    if (!IsValid(Avatar) || !IsValid(SourceASC))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 2. Setup Sphere Trace parameters
    const FVector Origin = Avatar->GetActorLocation();
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Avatar);

    EDrawDebugTrace::Type DebugType = bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    // 3. Perform Multi-Sphere Trace for specified Object Types
    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        Origin,         // Start
        Origin,         // End (static sphere)
        AbilityRadius,
        ObjectTypes,
        false,          // bTraceComplex
        ActorsToIgnore,
        DebugType,
        HitResults,
        true,           // bIgnoreSelf
        FLinearColor::Red, FLinearColor::Green, 2.0f
    );

    // Exit early if nothing was hit
    if (!bHit)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // 4. Prepare Gameplay Effect Spec
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

    if (!SpecHandle.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // 5. Apply Effect to all valid targets found in the trace
    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!IsValid(HitActor)) continue;

        const float FinalDamage = UGameplayStatics::ApplyDamage(HitActor, 
            Damage, 
            Avatar->GetInstigatorController(), 
            Avatar, 
            DamageTypeClass);

        SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("GAS.Stats.Damage"), FinalDamage);

        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
        if (IsValid(TargetASC))
        {
            SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

            UE_LOG(LogTemp, Log, TEXT("Radial Ability Hit: %s"), *HitActor->GetName());
        }
    }

    // 6. Finish Ability
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}