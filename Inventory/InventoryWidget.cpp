// Copyright © 2024, Alan Aldred. Ginger Beard Gaming Studios. GingerBeardGamingStudios.com

#include "InventoryWidget.h"

// Engine includes.
#include "Kismet/GameplayStatics.h"

// Custom includes.
#include "../Characters/Playable/AdventureCharacterBase.h"

UInventoryWidget::UInventoryWidget()
{
	DisplayName = "Inventory";
}


UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	// Get the player character
	TWeakObjectPtr<ACharacter> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInventoryComponent: PlayerCharacter was invalid!"));
		return nullptr;
	}

	// Cast to adventure character type so that we can get the inventory component.
	TWeakObjectPtr<AAdventureCharacterBase> AdventureCharacter = Cast<AAdventureCharacterBase>(PlayerCharacter);
	if (!AdventureCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInventoryComponent: AdventureCharacter was invalid!"));
		return nullptr;
	}

	// Get inventory component
	return AdventureCharacter->GetInventoryComponent();
}
