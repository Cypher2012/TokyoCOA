// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryInterface.generated.h"

class UInventoryWidget;
class UInventoryComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class IInventoryInterface
{
	GENERATED_BODY()

public:

	// Return a pointer to the inventory component.
	virtual UInventoryComponent* GetInventoryComponent();

	// Inventory updated.
	virtual void OnInventoryUpdated();
};