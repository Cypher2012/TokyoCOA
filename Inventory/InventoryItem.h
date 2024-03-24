// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InventoryItem.generated.h"

class UCraftRecipe;

// Base class for all inventory items
UCLASS()
class TOKYOCOA_API AInventoryItem : public AActor
{
	GENERATED_BODY()

public:

	// Gets if this item can be equipped.
	bool GetCanBeEquipped() const;

	// Gets the display name of this item.
	FName GetDisplayName() const;

	// Get the carry weight of this item in grams.
	float GetCarryWeight() const;

protected:

	// The display name of this item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	FName DisplayName = "DisplayName";

	// Recipe used when dismantling this inventory item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	TSubclassOf<UCraftRecipe> CraftRecipe = nullptr;

	// If this item is equippable.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	bool bCanBeEquipped = false;

	// If this item can be stacked.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	bool bCanBeStacked = false;

	// Carry weight of the item in grams.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float CarryWeight = 100.0f;

private:

};
