
#pragma once

#include "CoreMinimal.h"

// An enum to keep track of what race characters belong to
UENUM(BlueprintType)
enum ECharacterRace : uint8
{
	EHuman, // Used for normal human AI and the player character
	EMutant, // Used for mutant AI
	EAnimal // Used for random animals in the world
};