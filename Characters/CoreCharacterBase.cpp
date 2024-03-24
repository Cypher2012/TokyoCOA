// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "CoreCharacterBase.h"

// Engine includes
#include "AbilitySystemComponent.h"

// Custom includes
#include "../Inventory/InventoryComponent.h"

ACoreCharacterBase::ACoreCharacterBase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	CharacterCoreAttributes = CreateDefaultSubobject<UCharacterCoreAttributes>("CharacterAttributeSet");

	// Set up event for when attributes are changed.
	if (IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(CharacterCoreAttributes->GetHealthAttribute()).AddUObject(this, &ACoreCharacterBase::OnHealthChanged);
	}	

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

UAbilitySystemComponent* ACoreCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACoreCharacterBase::BeginPlay()
{
	Super::BeginPlay();


}

void ACoreCharacterBase::OnZeroHealth()
{
	bCharacterAlive = false;
}

void ACoreCharacterBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		OnZeroHealth();
	}
}
