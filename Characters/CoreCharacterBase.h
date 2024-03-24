// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

// Custom includes
#include "Enums/CharacterRace.h"
#include "GameplayTagContainer.h"
#include "CharacterCoreAttributes.h"

#include "CoreCharacterBase.generated.h"

class UInventoryComponent;

// Base class for all the characters in the game
UCLASS()
class TOKYOCOA_API ACoreCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	ACoreCharacterBase();	

protected:

	// If the character is still alive.
	bool bCharacterAlive = true;

	// Character race that this character belongs too.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characters")
	TEnumAsByte<ECharacterRace> CharacterRace = ECharacterRace::EHuman;

	// Races that this character is friendly with.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characters")
	TArray<TEnumAsByte<ECharacterRace>> FriendlyRaces;

	// Races that this character is enemies with.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characters")
	TArray<TEnumAsByte<ECharacterRace>> EnemyRaces;

	// Character core attributes.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	const UCharacterCoreAttributes* CharacterCoreAttributes;

	// Ability system component. Used primarily for the camp building.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* ASC;

	// Core character inventory component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UInventoryComponent* InventoryComponent = nullptr;

	virtual void BeginPlay() override;

	// Function to be called when damage received and health at zero
	virtual void OnZeroHealth();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

private:

	
};
