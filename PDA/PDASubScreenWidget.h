// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes.
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Custom includes.

#include "PDASubScreenWidget.generated.h"

// This is the widget used for individual screens that make up the PDA
UCLASS()
class TOKYOCOA_API UPDASubScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Returns the display name given to this PDA screen.
	UFUNCTION(BlueprintPure, Category = "PDA")
	FName GetDisplayName() const;

protected:

	// The display name for the screen. This will be used for building the menus etc.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	FName DisplayName = "Screen01";

private:


	
};
