// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resources/ResourceData.h"
#include "BaseResourcePickup.generated.h"

UCLASS()
class ABaseResourcePickup : public AActor
{
    GENERATED_BODY()

public:
    ABaseResourcePickup();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UResourceData* ResourceSettings;

protected:
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    UPROPERTY(VisibleAnywhere)
    class UPaperSpriteComponent* SpriteComp;
};
