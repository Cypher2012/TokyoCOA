#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"

// Custom classes

#include "BuildableItemDatabase.generated.h"

class ABuildableItem;

// Parent tree - no owners as this is a static class and just a place to declare some structs.

// Enum used for the item categories
// UBuildableItemDatabase contains a utility function to get a game ready display name for each category.
// It must be manually updated whenever adding a new enum!
UENUM(BlueprintType)
enum class EBuildableItemCategory : uint8
{
	EFoundations,
	EDoorFrames,	
	EWalls,
	EWindows,
	EFloors,
	ERoofs,	
	EStairs,

	// WARNING: Remember to update UBuildableItemDatabase::GetItemCategoryDisplayName() when adding a new category!

	// Dummy enum used to count number of categories. Items should not use this dummy category!
	ENumCategories
};

// Struct used to define data table that will hold info on all items
USTRUCT(BlueprintType)
struct FBuildableItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	// The display name of this item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SettlementBuilding")
	FName ItemDisplayName = "None";

	// The category that this item will belong too.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SettlementBuilding")
	EBuildableItemCategory ItemCategory = EBuildableItemCategory::EFloors;

	// The class of the buildable item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SettlementBuilding")
	TSubclassOf<ABuildableItem> ItemClass = nullptr;

};

// This class is just a static class to make accessing the items database easier.
UCLASS(Blueprintable)
class TOKYOCOA_API UBuildableItemDatabase : public UObject
{
	GENERATED_BODY()
	
public:

	// The number of categories found in EBuildableItemCategory.
	static int32 GetNumCategories();

	// Creates an array containing all the found rows with the matching category
	static void GetBuildableItemsByCategory(UDataTable* ItemsDataTable, EBuildableItemCategory ItemCategory, TArray<FBuildableItemTableRow>& FoundItems);

	// Reads the data table and stores an array of items for each catgories. The index of the outer array will be the index of the category!
	static void MapBuildableItems(UDataTable* ItemsDataTable, TArray<TArray<FBuildableItemTableRow>> &BuildableItemsArray);

	// Static functin to get a display name for each category. WARNING: Must be updated when a new category is added to EBuildableItemCategory.
	static FName GetItemCategoryDisplayName(EBuildableItemCategory Category);

	// Converts an index to an enum 
	static EBuildableItemCategory GetBuildableItemCategoryFromIndex(int32 CategoryIndex);
};
