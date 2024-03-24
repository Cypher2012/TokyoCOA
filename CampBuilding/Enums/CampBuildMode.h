
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECampBuildMode : uint8
{
	ECampObjectMode, // When spawning objects
	ECampEditMode // When editing existing items
};