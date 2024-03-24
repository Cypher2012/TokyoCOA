// Fill out your copyright notice in the Description page of Project Settings.


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
