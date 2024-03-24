// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "BuildableItemDatabase.h"

#include "UObject/Class.h"

#include "BuildableItem.h"


int32 UBuildableItemDatabase::GetNumCategories()
{
	return static_cast<int32>(EBuildableItemCategory::ENumCategories);
}

void UBuildableItemDatabase::MapBuildableItems(UDataTable* ItemsDataTable, TArray<TArray<FBuildableItemTableRow>> &BuildableItemsArray)
{
	// Clear array so that we are not stacking the same items over and over.
	BuildableItemsArray.Empty();

	// Loop through each category and store the items found in that category.
	for (int32 CategoryIndex = 0; CategoryIndex < UBuildableItemDatabase::GetNumCategories(); CategoryIndex++)
	{
		// Get the current category enum by casting the enum by index
		EBuildableItemCategory CurrentCategory = static_cast<EBuildableItemCategory>(CategoryIndex);
		TArray<FBuildableItemTableRow> FoundItems;
		GetItemsByCategory(ItemsDataTable, CurrentCategory, FoundItems);

		// Add the array of items for this category, to the outer array.
		BuildableItemsArray.Add(FoundItems);
	}
}

FName UBuildableItemDatabase::GetItemCategoryDisplayName(EBuildableItemCategory Category)
{
	switch (Category)
	{
	case EBuildableItemCategory::EFoundations:
		return FName("Foundations");
	case EBuildableItemCategory::EDoorFrames:
		return FName("Door Frames");
	case EBuildableItemCategory::EWalls:
		return FName("Walls");
	case EBuildableItemCategory::EWindows:
		return FName("Windows");
	case EBuildableItemCategory::EFloors:
		return FName("Floors");		
	case EBuildableItemCategory::ERoofs:
		return FName("Roofs");
	case EBuildableItemCategory::EStairs:
		return FName("Stairs");
	
	default:
		return FName("CategoryUnNamed");
	}	
}

void UBuildableItemDatabase::GetItemsByCategory(UDataTable* ItemsDataTable, EBuildableItemCategory ItemCategory, TArray<FBuildableItemTableRow>& FoundItems)
{
	// check if a valid data table was passed.
	if (!IsValid(ItemsDataTable))
	{
		return;
	}

	// Empty the array so that we don't stack items.
	FoundItems.Empty();

	// Loop through all the rows in the data table.
	for (TPair<FName, uint8*> RowPairs : ItemsDataTable->GetRowMap())
	{
		// Cast to the correct data table row type.
		FBuildableItemTableRow* ItemTableRow = (FBuildableItemTableRow*)RowPairs.Value;

		// Check if the category matches the ItemCategory.
		if (ItemTableRow->ItemCategory == ItemCategory)
		{
			FoundItems.Add(*ItemTableRow);
		}
	}
}