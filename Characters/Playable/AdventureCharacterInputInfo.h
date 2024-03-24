// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"

#include "InputActionValue.h"

#include "AdventureCharacterInputInfo.generated.h"

class UInputMappingContext;
class UInputAction;

// All info related to the characters input 
USTRUCT(BlueprintType)
struct FAdventureCharacterInputInfo
{
	GENERATED_BODY()

public:

	// Input context for the build controls
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* BuildingMappingContext = nullptr;

	// Input action for toggling the build controls on and off.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleBuildControls = nullptr;

	// Input action to toggle the inventory screen
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleShowInventory = nullptr;

};