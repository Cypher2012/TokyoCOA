// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemInterface.h"

// Custom classes
#include "../Inventory/InventoryInterface.h"

#include "CoreGameHUD.generated.h"

class UMainGameWidget;
class UCampBuildingWidget;

// The core HUD of the game. This will be the main controller for all the sub HUD's.
// It will be responsible for displaying and hiding main game hud and camp build HUD.
UCLASS()
class TOKYOCOA_API ACoreGameHUD : public AHUD, public IInventoryInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	// Return a pointer to the inventory component.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent();

	// Get a pointer to the players ability system.
	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnInventoryUpdated() override;

	// Re show the main game widget.
	void DisplayMainGameWidget();

	// hides the main game widget
	void HideMainGameWidget();

	// A method used to create and display the settlement build widget.
	void DisplayCampBuildingWidget();

	// Removes the settlement build widget from the viewport and displays the main game widget
	void RemoveCampBuildingWidget();

	// Called by player character on death to show the widget for when player dies.
	void ShowPlayerDeathWidget();

	// Returns a pointer to the camp build widget
	TWeakObjectPtr<UCampBuildingWidget> GetCampBuildWidget() const;

protected:

	// Widget class used for the main game HUD
	// Main game HUD will be built once and shown/hidden
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
	TSubclassOf<UMainGameWidget> MainGameWidgetClass = nullptr;

	// Widget used for the camp building HUD
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
	TSubclassOf<UCampBuildingWidget> CampBuildingWidgetClass = nullptr;

	// The widget that will be displayed when use dies
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
	TSubclassOf<UUserWidget> PlayerDeathWidgetClass = nullptr;

	virtual void BeginPlay() override;

private:

	// A hard reference to the main game widget.
	UMainGameWidget* MainGameWidget = nullptr;	

	// A hard reference to the settlement build widget.
	UCampBuildingWidget* CampBuildingWidget = nullptr;

	// Creates the main game widget and adds it to the viewport.
	void CreateMainGameWidget();

	// Removes all the widgets
	void RemoveAllGameWidgets();
};
