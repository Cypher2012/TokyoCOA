#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"

#include "AdventureCharacterAbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAdventureCharacterAbilityInfo
{
	GENERATED_BODY()

public:

	FAdventureCharacterAbilityInfo();

	// Tag to check if the player can start spawning a camp.
	FGameplayTag CharacterCanSpawnCampTag = FGameplayTag();

	// Tag to check if the character is within the bounds of a camp.
	FGameplayTag CharacterInCampBoundsTag = FGameplayTag();

	// Handle to the camp spawn ability.
	FGameplayAbilitySpecHandle CampSpawnerAbilityHandle = FGameplayAbilitySpecHandle();

	// Handle to the camp builder ability.
	FGameplayAbilitySpecHandle CampBuilderAbilityHandle = FGameplayAbilitySpecHandle();

	// Handle to the open PDA ability.
	FGameplayAbilitySpecHandle PlayerPDAAbilityHandle = FGameplayAbilitySpecHandle();

	// The ability class used for building in camps.
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TSubclassOf<UGameplayAbility> CampBuildingAbilityClass = nullptr;

	// The ability class used for spawning camps.
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TSubclassOf<UGameplayAbility> CampSpawnerAbilityClass = nullptr;

	// The ability class used for opening the player PDA.
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TSubclassOf<UGameplayAbility> PlayerPDAAbilityClass = nullptr;
};