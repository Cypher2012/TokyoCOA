// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CraftRecipe.generated.h"

class ACraftResource;

// recipes that are used for inventory items
UCLASS(Blueprintable)
class TOKYOCOA_API UCraftRecipe : public UObject
{
	GENERATED_BODY()

public:

protected:

	// An array of required resources and the number required.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TMap<TSubclassOf<ACraftResource>, int32> RequiredResources;

private:

};
