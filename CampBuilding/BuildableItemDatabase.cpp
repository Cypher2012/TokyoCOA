// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildableItemDatabase.h"

#include "UObject/Class.h"

#include "BuildableItem.h"


int32 UBuildableItemDatabase::GetNumCategories()
{
	return static_cast<int32>(EBuildableItemCategory::ENumCategories);
}

void UBuildableItemDatabase::GetBuildableItemsByCategory(UDataTable* ItemsDataTable, EBuildableItemCategory ItemCategory, TArray<FBuildableItemTableRow>& FoundItems)
{
	if (!IsValid(ItemsDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetBuildableItemsByCategory: ItemsDataTable was invalid!"));
		return;
	}

	TArray<FBuildableItemTableRow> TempRows;
	TempRows.Empty();
	
	for (TPair<FName, uint8*> RowPairs : ItemsDataTable->GetRowMap())
	{
		FBuildableItemTableRow* ItemData = (FBuildableItemTableRow*)RowPairs.Value;
		if (ItemData->ItemCategory == ItemCategory)
		{
			TempRows.Add(*ItemData);
		}
	}	

	FoundItems = TempRows;
}

void UBuildableItemDatabase::MapBuildableItems(UDataTable* ItemsDataTable, TArray<TArray<FBuildableItemTableRow>> &BuildableItemsArray)
{
	// Create a new empty TMap
	TArray<TArray<FBuildableItemTableRow>> TempBuildableItems;
	TempBuildableItems.Empty();

	// Loop through each category 
	for (int32 CategoryIndex = 0; CategoryIndex < UBuildableItemDatabase::GetNumCategories(); CategoryIndex++)
	{

		// Get the current category enum by casting the enum by index
		EBuildableItemCategory CurrentCategory = static_cast<EBuildableItemCategory>(CategoryIndex);

		TArray<FBuildableItemTableRow> FoundItems;
		GetBuildableItemsByCategory(ItemsDataTable, CurrentCategory, FoundItems);

		// Add the array of items for this category, to the outer array.
		TempBuildableItems.Add(FoundItems);
		
	}

	// Return the filled map
	BuildableItemsArray = TempBuildableItems;

	UE_LOG(LogTemp, Warning, TEXT("MapBuildableItemCategories: Mapping buildable objects data."));
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

EBuildableItemCategory UBuildableItemDatabase::GetBuildableItemCategoryFromIndex(int32 CategoryIndex)
{
	return static_cast<EBuildableItemCategory>(CategoryIndex);
}
