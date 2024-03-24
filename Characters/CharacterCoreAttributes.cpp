// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterCoreAttributes.h"

#include "GameplayEffectExtension.h"


void UCharacterCoreAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
	// Clamp health attribute	
	if (Attribute.AttributeName == "Health")
	{
		NewValue = FMath::Clamp(NewValue, 0, MaxHealth.GetBaseValue());
	}

	// Clamp stamina attribute.
	if (Attribute.AttributeName == "Stamina")
	{
		NewValue = FMath::Clamp(NewValue, 0, MaxStamina.GetBaseValue());
	}
	
}