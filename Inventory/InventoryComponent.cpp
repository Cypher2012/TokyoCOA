// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

// Engine classes
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Custom includes
#include "InventoryItem.h"
#include "InventoryAttributes.h"
#include "../Characters/CoreCharacterBase.h"
#include "../PlayerHUD/CoreGameHUD.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InventoryAttributes = CreateDefaultSubobject<UInventoryAttributes>("InventoryAttributes");
}

UAbilitySystemComponent* UInventoryComponent::GetAbilitySystemComponent() const
{
	TWeakObjectPtr<ACoreCharacterBase> OwningCharacter = Cast<ACoreCharacterBase>(GetOwner());
	if (!OwningCharacter.IsValid())
	{
		return nullptr;
	}

	return OwningCharacter->GetAbilitySystemComponent();
}

UInventoryComponent* UInventoryComponent::GetInventoryComponent()
{
	return this;
}

ACoreGameHUD* UInventoryComponent::GetCoreGameHUD()
{
	TWeakObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController.IsValid())
	{
		return nullptr;
	}

	return Cast<ACoreGameHUD>(PlayerController->GetHUD());	
}

void UInventoryComponent::DebugPrintInventory()
{
	FString OutputString = "";
		
	for (AInventoryItem* InventoryItem : InstancedItems)
	{
		if (IsValid(InventoryItem))
		{
			OutputString += "Instanced ItemName: " + InventoryItem->GetDisplayName().ToString();
			OutputString += "\n";			
		}
	}	

	for (TPair NonInstancedItem : NonInstancedItems)
	{
		OutputString += "NonInstancedItem Class: " + NonInstancedItem.Key.GetDefaultObject()->GetDisplayName().ToString();
		OutputString += " | Count: " + FString::FromInt(NonInstancedItem.Value);
		OutputString += "\n";
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, OutputString);	
}

bool UInventoryComponent::TryAddInstancedItem(AInventoryItem* NewInstancedItem, int32& OutItemIndex)
{
	if (!HasAvailableCarryWeight(GetItemCarryWeight(NewInstancedItem, nullptr)))
	{
		return false;
	}

	AddInstancedItem(NewInstancedItem, OutItemIndex);

	return true;
}

bool UInventoryComponent::TryAddNonInstancedItem(TSubclassOf<AInventoryItem> NewItemClass, int32& OutItemCount)
{
	if (!HasAvailableCarryWeight(GetItemCarryWeight(nullptr, NewItemClass)))
	{
		return false;
	}

	AddNonInstancedItem(NewItemClass, OutItemCount);
	return true;
}

float UInventoryComponent::GetTotalCarryWeight() const
{
	float TotalCarryWeight = 0;

	for (AInventoryItem* InstancedItem : InstancedItems)
	{
		if (IsValid(InstancedItem))
		{
			TotalCarryWeight += InstancedItem->GetCarryWeight();
		}
	}

	for (TPair NonInstancedItem : NonInstancedItems)
	{
		if (IsValid(NonInstancedItem.Key))
		{
			// Get defualt object
			AInventoryItem* DefaultItem = NonInstancedItem.Key->GetDefaultObject<AInventoryItem>();

			// Get carry weight
			if (IsValid(DefaultItem))
			{
				float CarryWeight = DefaultItem->GetCarryWeight();

				// Multiply that by the count
				CarryWeight *= NonInstancedItem.Value;

				// Add to total carry weight
				TotalCarryWeight += CarryWeight;
			}			
		}
	}

	return TotalCarryWeight;
}

float UInventoryComponent::GetMaxCarryWeight() const
{
	if (!IsValid(GetAbilitySystemComponent()))
	{
		return 0;
	}

	bool bAttributeFound;
	return GetAbilitySystemComponent()->GetGameplayAttributeValue(InventoryAttributes->GetMaxCarryWeightAttribute(), bAttributeFound);
}

TArray<AInventoryItem*> UInventoryComponent::GetInstancedItems() const
{
	return InstancedItems;
}

TMap<TSubclassOf<AInventoryItem>, int32> UInventoryComponent::GetNonInstancedItems() const
{
	return NonInstancedItems;
}

void UInventoryComponent::AddInstancedItem(AInventoryItem* NewInstancedItem, int32& OutItemIndex)
{
	if (!IsValid(NewInstancedItem))
	{
		return;
	}

	OutItemIndex = InstancedItems.AddUnique(NewInstancedItem);

	OnInventoryUpdated();
}

void UInventoryComponent::AddNonInstancedItem(TSubclassOf<AInventoryItem> NewItemClass, int32& OutItemCount)
{
	if (!IsValid(NewItemClass))
	{
		return;
	}	

	// Stack the item if it already exists, or add a new entry.
	int32* NumItems = NonInstancedItems.Find(NewItemClass);
	if (NumItems)
	{
		OutItemCount = NonInstancedItems.Add(NewItemClass, *NumItems + 1);
	}
	else
	{
		OutItemCount = NonInstancedItems.Add(NewItemClass, 1);
	}

	OnInventoryUpdated();
}

bool UInventoryComponent::HasAvailableCarryWeight(int32 CarryWeight)
{
	if (GetTotalCarryWeight() + CarryWeight >= GetMaxCarryWeight())
	{
		return false;
	}
	
	return true;
}

int32 UInventoryComponent::GetItemCarryWeight(AInventoryItem* InstancedItem, TSubclassOf<AInventoryItem> ItemClass)
{
	// Neither instanced item or item class are valid. So nothing was passed. Return 0
	if (!IsValid(InstancedItem) && !IsValid(ItemClass))
	{
		return 0;
	}

	// Get the carry weight of the instanced item.
	if (IsValid(InstancedItem))
	{
		return InstancedItem->GetCarryWeight();
	}

	// Create a default object of the item class and then get the carry weight from that.
	if (IsValid(ItemClass))
	{
		AInventoryItem* DefaultItem = ItemClass->GetDefaultObject<AInventoryItem>();
		if (!IsValid(DefaultItem))
		{
			return 0;
		}
		return DefaultItem->GetCarryWeight();
	}

	return 0;
}
