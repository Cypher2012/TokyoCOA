// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "PDAContainerPanel.h"

#include "PDASubScreenWidget.h"

void UPDAContainerPanel::DisplaySelectedScreen()
{
	// Destroy the current widget inside of the container
	if (IsValid(DisplayedScreen))
	{
		DisplayedScreen->RemoveFromParent();
		DisplayedScreen = nullptr;
	}

	// Check current index is valid
	if (!ScreenWidgetClasses.IsValidIndex(CurrentScreenIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("DisplaySelectedScreen: Index is invalid!"));
		return;
	}

	// Get the class that is at that index
	TSubclassOf<UPDASubScreenWidget> ScreenWidgetClass = ScreenWidgetClasses[CurrentScreenIndex];
	if (!IsValid(ScreenWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("DisplaySelectedScreen: Screen widget class is invalid!"));
		return;
	}

	// Create the widget of that class.
	DisplayedScreen = CreateWidget<UPDASubScreenWidget>(this, ScreenWidgetClass);
	if (!IsValid(DisplayedScreen))
	{
		UE_LOG(LogTemp, Warning, TEXT("DisplaySelectedScreen: NewScreenWidget was invalid!"));
		return;
	}

	// Add the new widget to the screen container.	
	AddChild(DisplayedScreen);	
}

int32 UPDAContainerPanel::ScrollScreen(bool bScrollNext)
{
	if (bScrollNext)
	{
		CurrentScreenIndex++;

		if (CurrentScreenIndex >= ScreenWidgetClasses.Num())
		{
			CurrentScreenIndex = 0;
		}
	}
	else
	{
		CurrentScreenIndex--;

		if (CurrentScreenIndex < 0)
		{
			CurrentScreenIndex = ScreenWidgetClasses.Num() - 1;
		}
	}

	DisplaySelectedScreen();

	return CurrentScreenIndex;
}
