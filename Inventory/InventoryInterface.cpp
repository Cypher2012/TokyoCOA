// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "InventoryInterface.h"

#include "InventoryComponent.h"

UInventoryComponent* IInventoryInterface::GetInventoryComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("IInventoryInterface::GetInventoryComponent"));
	return nullptr;
}

void IInventoryInterface::OnInventoryUpdated()
{
	//UE_LOG(LogTemp, Warning, TEXT("IInventoryInterface::OnInventoryUpdated"));
}
