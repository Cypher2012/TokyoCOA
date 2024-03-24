// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

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