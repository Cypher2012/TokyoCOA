// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes.
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"

// Custom includes


#include "PDAContainerPanel.generated.h"

class UPDASubScreenWidget;

// This is the main container panel that manages and displays the PDA screens
UCLASS()
class TOKYOCOA_API UPDAContainerPanel : public UCanvasPanel
{
	GENERATED_BODY()

public:


protected:

	// An array of the individual screens that make up the PDA
	// This is how we actually add new screens to the PDA!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PDA")
	TArray<TSubclassOf<UPDASubScreenWidget>> ScreenWidgetClasses = TArray<TSubclassOf<UPDASubScreenWidget>>();

	// Displays the currently selected screen inside of the container widget.
	UFUNCTION(BlueprintCallable, Category = "PDA")
	void DisplaySelectedScreen();

	// Scrolls through the available screens.
	UFUNCTION(BlueprintCallable, Category = "PDA")
	int32 ScrollScreen(bool bScrollNext = true);

private:

	// Index of the currently displayed screen.
	int32 CurrentScreenIndex = 0;

	// Pointer to the widget screen that is currently being displayed.
	UPDASubScreenWidget* DisplayedScreen = nullptr;
	
};
