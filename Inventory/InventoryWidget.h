// Copyright © 2024, Alan Aldred. Ginger Beard Gaming Studios. GingerBeardGamingStudios.com

#pragma once

// Engine includes
#include "CoreMinimal.h"

// Custom includes
#include "InventoryInterface.h"
#include "../PDA/PDASubScreenWidget.h"

#include "InventoryWidget.generated.h"

// The inventory screen that will be added to the PDA screens.
UCLASS()
class TOKYOCOA_API UInventoryWidget : public UPDASubScreenWidget , public IInventoryInterface
{
	GENERATED_BODY()
	
public:

	UInventoryWidget();
	
	// Return a pointer to the inventory component.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent();	

protected:

private:

};
