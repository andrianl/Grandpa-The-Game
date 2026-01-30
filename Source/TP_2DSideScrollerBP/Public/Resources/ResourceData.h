// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include <PaperSprite.h>
#include "ResourceData.generated.h"


UCLASS(BlueprintType)
class UResourceData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

    UResourceData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    UPaperSprite* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    FText ResourceName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    FGameplayAttribute ResourceAttribute;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    float AmountToAdd = 1.0f;
};
