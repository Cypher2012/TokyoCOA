// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"

// Custom includes
#include "../PlayerHUD/GameHUDInterface.h"
#include "../Inventory/InventoryInterface.h"

#include "InventoryComponent.generated.h"

class AInventoryItem;
class UInventoryAttributes;

// Main inventory component for characters. This will handle general items but also resources for building.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOKYOCOA_API UInventoryComponent : public UActorComponent, public IAbilitySystemInterface, public IGameHUDInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	// Gets the owners ability system component.
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Return a pointer to the inventory component.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent() override;

	// Get a weak pointer to the core game HUD
	UFUNCTION(BlueprintCallable, Category = "Player HUD")
	virtual ACoreGameHUD* GetCoreGameHUD() override;

	// Debug function to print out the inventory.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DebugPrintInventory();

	// Tries to add an instanced item to the inventory. Checks available carry weight before calling AddInstancedItem().
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddInstancedItem(AInventoryItem* NewInstancedItem, int32 &OutItemIndex);

	// Tries to add the item class to the array of non instanced items. Checks available carry weight before calling AddNonInstancedItem.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddNonInstancedItem(TSubclassOf<AInventoryItem> NewItemClass, int32& OutItemCount);

	// Calculates the total carry weight of all the items in the inventory.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetTotalCarryWeight() const;

	// Get the current max carry weight from the InventoryAttributes MaxCarryWeight.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetMaxCarryWeight() const;

	// Returns an array of all the instanced items in the inventory.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<AInventoryItem*> GetInstancedItems() const;

	// Returns a map containing all the non instanced item classes, the "value" associated with the key will be the item count.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TMap<TSubclassOf<AInventoryItem>, int32> GetNonInstancedItems() const;

protected:

	// Gameplay attributes for the inventory component.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	UInventoryAttributes* InventoryAttributes = nullptr;

private:

	// An array of instanced inventory items. The are items that need to be persistant such as weapons.
	// Instanced items are unique and can not be stacked.
	TArray<AInventoryItem*> InstancedItems = TArray<AInventoryItem*>();

	// A map of non instanced items and the count. This is for items that don't need to be instanced such as resource items. 
	// Non instanced items can be stacked. They are just a class reference and do not exist in the world.
	TMap<TSubclassOf<AInventoryItem>, int32> NonInstancedItems = TMap<TSubclassOf<AInventoryItem>, int32>();

	// Adds the pointer to the array of instanced items. Retuns the index of the added item.
	void AddInstancedItem(AInventoryItem* NewInstancedItem, int32& OutItemIndex);

	// Checks if we have the item class already in the inventory. Will increase the item count of add a new item. Returns the item count.
	void AddNonInstancedItem(TSubclassOf<AInventoryItem> NewItemClass, int32& OutItemCount);

	// Check to see if we have carry weight available. Returns true if available carry weight is >= to CarryWeight
	bool HasAvailableCarryWeight(int32 CarryWeight);

	// Gets the weight of the given item. Either pass in a pointer to an instanced item, or the class of the item you want to check.
	int32 GetItemCarryWeight(AInventoryItem* InstancedItem = nullptr, TSubclassOf<AInventoryItem> ItemClass = nullptr);
};
