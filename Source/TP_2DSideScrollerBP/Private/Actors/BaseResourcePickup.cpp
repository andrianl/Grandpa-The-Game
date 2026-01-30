// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseResourcePickup.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "PaperSpriteComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseResourcePickup::ABaseResourcePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// BaseResourcePickup.cpp
void ABaseResourcePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (!IsValid(ResourceSettings)) return;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

    if (IsValid(ASC))
    {
        ASC->ApplyModToAttribute(ResourceSettings->ResourceAttribute, EGameplayModOp::Additive, ResourceSettings->AmountToAdd);

        Destroy();
    }
}

void ABaseResourcePickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (IsValid(ResourceSettings) && IsValid(SpriteComp))
    {
        SpriteComp->SetSprite(ResourceSettings->Icon);
    }
}

