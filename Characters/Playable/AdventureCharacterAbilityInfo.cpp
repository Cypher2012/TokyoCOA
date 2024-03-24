#include "AdventureCharacterAbilityInfo.h"

#include "GameplayTagContainer.h"

FAdventureCharacterAbilityInfo::FAdventureCharacterAbilityInfo()
{
	CharacterCanSpawnCampTag = FGameplayTag::RequestGameplayTag("CampBuilding.CharacterCanSpawnCamp");
	CharacterInCampBoundsTag = FGameplayTag::RequestGameplayTag("CampBuilding.CharacterInCampBounds");
}
