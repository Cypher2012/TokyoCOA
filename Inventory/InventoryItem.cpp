// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "InventoryItem.h"

#include "CraftRecipe.h"

bool AInventoryItem::GetCanBeEquipped() const
{
    return bCanBeEquipped;
}

FName AInventoryItem::GetDisplayName() const
{
    return DisplayName;
}

float AInventoryItem::GetCarryWeight() const
{
    return CarryWeight;
}
