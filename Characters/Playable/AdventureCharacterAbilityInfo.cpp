// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "AdventureCharacterAbilityInfo.h"

#include "GameplayTagContainer.h"

FAdventureCharacterAbilityInfo::FAdventureCharacterAbilityInfo()
{
	CharacterCanSpawnCampTag = FGameplayTag::RequestGameplayTag("CampBuilding.CharacterCanSpawnCamp");
	CharacterInCampBoundsTag = FGameplayTag::RequestGameplayTag("CampBuilding.CharacterInCampBounds");
}
