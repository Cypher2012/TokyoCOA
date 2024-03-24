// Fill out your copyright notice in the Description page of Project Settings.


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
