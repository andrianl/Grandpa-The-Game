// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ResourceAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class TP_2DSIDESCROLLERBP_API UResourceAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UResourceAttributeSet();

protected:
	/** Sample "Mana" Attribute */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Apples;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxApples;

public:
	//~ Helper functions for "Apples" attributes
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UResourceAttributeSet, Apples);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Apples);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Apples);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Apples);

	//~ Helper functions for "MaxApples" attributes
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UResourceAttributeSet, MaxApples);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxApples);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxApples);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxApples);
};
