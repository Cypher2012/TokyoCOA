// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"


#include "CharacterCoreAttributes.generated.h"

// Core attribute set used for the game characters.
UCLASS()
class TOKYOCOA_API UCharacterCoreAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCharacterCoreAttributes, Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCharacterCoreAttributes, MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCharacterCoreAttributes, Stamina);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Stamina);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:

private:

	// Current health of the character.
	UPROPERTY()
	FGameplayAttributeData Health;

	// Current health of the character.
	UPROPERTY()
	FGameplayAttributeData MaxHealth;

	// Current stamina on the character.
	UPROPERTY()
	FGameplayAttributeData Stamina;

	// Current health of the character.
	UPROPERTY()
	FGameplayAttributeData MaxStamina;
};
