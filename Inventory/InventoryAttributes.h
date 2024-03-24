// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "InventoryAttributes.generated.h"

// Main attribute set for the inventory component
UCLASS()
class TOKYOCOA_API UInventoryAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UInventoryAttributes, MaxCarryWeight);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxCarryWeight);

protected:

	// Max carry weight of the inventory
	UPROPERTY()
	FGameplayAttributeData MaxCarryWeight;

private:


	
};
