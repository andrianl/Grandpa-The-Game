// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/BaseResourcePickup.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PaperSpriteComponent.h"

ABaseResourcePickup::ABaseResourcePickup()
{
    // 1. Optimization: Pickups don't need to tick
    PrimaryActorTick.bCanEverTick = false;

    // 2. Initialize the Sprite Component
    SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
    RootComponent = SpriteComp;
}

void ABaseResourcePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    // Only handle resource collection on the Server
    if (GetLocalRole() != ROLE_Authority) return;

    if (!IsValid(ResourceSettings))
    {
        UE_LOG(LogTemp, Error, TEXT("Pickup %s is missing ResourceSettings!"), *GetName());
        return;
    }

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

    if (IsValid(ASC))
    {
        // Option A: Direct attribute modification (Fastest)
        ASC->ApplyModToAttribute(ResourceSettings->ResourceAttribute, EGameplayModOp::Additive, ResourceSettings->AmountToAdd);

        // Optional: Play sound or VFX here before destroying

        UE_LOG(LogTemp, Log, TEXT("Picked up %f of %s"), ResourceSettings->AmountToAdd, *ResourceSettings->ResourceName.ToString());

        Destroy();
    }
}

void ABaseResourcePickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Update the sprite visually as soon as we change the DataAsset in the editor
    if (IsValid(ResourceSettings) && IsValid(SpriteComp))
    {
        SpriteComp->SetSprite(ResourceSettings->Icon);
    }
}

