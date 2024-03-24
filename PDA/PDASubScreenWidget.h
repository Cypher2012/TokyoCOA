// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDASubScreenWidget.generated.h"

// This is the widget used for individual screens that make up the PDA
UCLASS()
class TOKYOCOA_API UPDASubScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// The display name for the screen. This will be used for building the menus etc.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	FName DisplayName = "Screen01";

protected:

private:


	
};
