// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes
#include "CoreMinimal.h"

// Custom includes
#include "../../CampBuilding/BuildableItemDatabase.h"

#include "BuildMenuSelection.generated.h"

// Struct used to hold information about the current menu selection
// This will be used along side TArray<TArray<FBuildableItemData>> BuildableItemsArray;
// The CategoryMapItemIndex will be the inner index of the array, containing the items in that category.
// The CategoryIndex will be the index of the outer array and be the index of the category.
// See GetSelectedItem()
USTRUCT(BlueprintType)
struct FBuildMenuSelection
{
	GENERATED_BODY()

public:

	// Index of the current category
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameHUD")
	int32 CategoryIndex = 0;

	// The index within the category map of the item
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameHUD")
	int32 ItemIndex = 0;

	// The currently selected item.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameHUD")
	FBuildableItemTableRow SelectedItem = FBuildableItemTableRow();
};