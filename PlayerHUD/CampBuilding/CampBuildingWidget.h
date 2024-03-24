// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Enhanced input includes
#include "InputActionValue.h"

// Custom.CampBuilding classes
#include "../../CampBuilding/BuildableItem.h"
#include "../../CampBuilding/BuildableItemDatabase.h"
#include "../../CampBuilding/Enums/CampBuildMode.h"

// Custom.General classes
#include "../Structs/BuildMenuSelection.h"

#include "CampBuildingWidget.generated.h"

// Enhanced input classes
class UInputMappingContext;
class UInputAction;

class UCampBuildingAbility;

// Parent tree - this/CoreGameHUD/AdventurePlayGameMode

// The main widget that is displayed when building in settlements
UCLASS()
class TOKYOCOA_API UCampBuildingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// Returns the item menu selection. FBuildMenuSelection will be the main way of communicating between 
	// widgets and the builder manager.
	UFUNCTION(BlueprintPure, Category = "ItemHUD")
	FBuildMenuSelection GetBuildMenuSelection() const;

	// Event called by the camp builder ability when the build mode changes
	void OnBuildModeChanged(const ECampBuildMode NewBuildMode);

	// Sets a pointer to the camp building ability.
	void SetCampBuildingAbility(TWeakObjectPtr<UCampBuildingAbility> InCampBuildingAbility);

protected:

	// Input context used for building.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* SettlementWidgetInputContext = nullptr;

	// Input action for scrolling to the next category.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ScrollCategoryNext = nullptr;

	// Input action to scroll to the previous category.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ScrollCategoryPrevious = nullptr;

	// Input action to scroll to the next item.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ScrollItemNext = nullptr;

	// Input action to scroll to the previous item.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ScrollItemPrevious = nullptr;

	// Current build mode player is using.
	UPROPERTY(BlueprintReadOnly, Category = "ItemHUD")
	ECampBuildMode CurrentBuildMode = ECampBuildMode::ECampObjectMode;

	// A weak reference to the data table holding data about the buildable items
	UPROPERTY(EditDefaultsOnly, Category = "Item HUD")
	UDataTable* BuildableItemDataTable = nullptr;

	// Event that the widget has been updated. To be implemented in the blueprint child widget.
	UFUNCTION(BlueprintNativeEvent, Category = "ItemHUD")
	void OnWidgetUpdated();

	// Get the name of the current selected category.
	UFUNCTION(BlueprintPure, Category = "ItemHUD")
	FName GetCategoryDisplayName(const int32 CategoryIndex) const;

private:

	// New array of buildable item classes. The first index will represent the index of the category.
	TArray<TArray<FBuildableItemTableRow>> BuildableItemsArray;

	// The current menu seleciton. Holds an index to the category and the item selection.
	FBuildMenuSelection BuildMenuSelection = FBuildMenuSelection();

	// ===== Input Setup =====

	// Set's up the input component
	void SetupInputComponent();

	// Scroll to the next category.
	void OnInputScrollCategoryNext();

	// Scroll to the previous category
	void OnInputScrollCategoryPrevious();

	// Input received to scroll to the next item.
	void OnInputScrollItemNext();

	// Input received to scroll to the previous item.
	void OnInputScrollItemPrevious();

	// ===== End of Input Setup =====

	// Gets the number of items in the the TArray of items under that category index.
	int32 GetNumItemsInCategory(const int32 CategoryIndex) const;

	// Updates the menu selection with the selected item. Sets it to nullptr if invalid.
	// Should be called whenever a change is made to the selected category or item index.
	void UpdateSelectedItem();

	// Checks if the CategoryIndex and ItemIndex is a valid index
	bool IsValidItemIndex(const int32 CategoryIndex, const int32 ItemIndex) const;

	// A weak pointer to the camp building ability.
	TWeakObjectPtr<UCampBuildingAbility> CampBuildingAbility;

};