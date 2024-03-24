// Fill out your copyright notice in the Description page of Project Settings.


#include "CampBuildingWidget.h"

// Engine classes
#include "Kismet/GameplayStatics.h"

// Enhanced input includes
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"

// Custom classes
#include "../../Characters/CoreCharacterBase.h"
#include "../../CampBuilding/Abilities/CampBuildingAbility.h"

void UCampBuildingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetupInputComponent();

	// Retrieve the items from the database and map them to the BuildableItemsArray.
	UBuildableItemDatabase::MapBuildableItems(BuildableItemDataTable, BuildableItemsArray);

	// Set the build menu selection to the first item of the first category.
	if (!IsValidItemIndex(0, 0))
	{
		return;
	}
	BuildMenuSelection.SelectedItem = BuildableItemsArray[0][0];

	// Send update event to the blueprint child of this class.
	OnWidgetUpdated();
}

void UCampBuildingWidget::OnWidgetUpdated_Implementation()
{
	UpdateSelectedItem();	
}

FName UCampBuildingWidget::GetCategoryDisplayName(const int32 CategoryIndex) const
{
	EBuildableItemCategory CategoryEnum = UBuildableItemDatabase::GetBuildableItemCategoryFromIndex(CategoryIndex);
	return UBuildableItemDatabase::GetItemCategoryDisplayName(CategoryEnum);
}

FBuildMenuSelection UCampBuildingWidget::GetBuildMenuSelection() const
{
	return BuildMenuSelection;
}

void UCampBuildingWidget::OnBuildModeChanged(const ECampBuildMode NewBuildMode)
{
	CurrentBuildMode = NewBuildMode;

	OnWidgetUpdated();
}

void UCampBuildingWidget::SetCampBuildingAbility(TWeakObjectPtr<UCampBuildingAbility> InCampBuildingAbility)
{
	CampBuildingAbility = InCampBuildingAbility;

	// Update the camp building ability of the build menu selection change
	if (CampBuildingAbility.IsValid())
	{
		CampBuildingAbility->OnBuildMenuSelectionChange(BuildMenuSelection);
	}
}

void UCampBuildingWidget::SetupInputComponent()
{
	// Create a new InputComponent of type UEnhancedInputComponent.
	InputComponent = NewObject<UEnhancedInputComponent>(this);	
	RegisterInputComponent();

	// Get the player controller. Needed so we can get the subsystem
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	// Get the enhanced input subsystem.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	// Add the settlement widget mapping context.
	Subsystem->AddMappingContext(SettlementWidgetInputContext, 0);

	// Cast input component to EnhancedInputComponent and then bind the actions.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		return;
	}

	// Finally, bind the actions.
	EnhancedInputComponent->BindAction(IA_ScrollCategoryNext, ETriggerEvent::Completed, this, &UCampBuildingWidget::OnInputScrollCategoryNext);
	EnhancedInputComponent->BindAction(IA_ScrollCategoryPrevious, ETriggerEvent::Completed, this, &UCampBuildingWidget::OnInputScrollCategoryPrevious);
	EnhancedInputComponent->BindAction(IA_ScrollItemNext, ETriggerEvent::Completed, this, &UCampBuildingWidget::OnInputScrollItemNext);
	EnhancedInputComponent->BindAction(IA_ScrollItemPrevious, ETriggerEvent::Completed, this, &UCampBuildingWidget::OnInputScrollItemPrevious);	
}

void UCampBuildingWidget::OnInputScrollCategoryNext()
{
	BuildMenuSelection.CategoryIndex += 1;
	BuildMenuSelection.CategoryIndex = FMath::Clamp(BuildMenuSelection.CategoryIndex, 0, BuildableItemsArray.Num() - 1);
	BuildMenuSelection.ItemIndex = 0;

	OnWidgetUpdated();
}

void UCampBuildingWidget::OnInputScrollCategoryPrevious()
{
	BuildMenuSelection.CategoryIndex -= 1;
	BuildMenuSelection.CategoryIndex = FMath::Clamp(BuildMenuSelection.CategoryIndex, 0, BuildableItemsArray.Num() - 1);
	BuildMenuSelection.ItemIndex = 0;

	OnWidgetUpdated();
}

void UCampBuildingWidget::OnInputScrollItemNext()
{
	BuildMenuSelection.ItemIndex += 1;
	BuildMenuSelection.ItemIndex = FMath::Clamp(BuildMenuSelection.ItemIndex, 0, (GetNumItemsInCategory(BuildMenuSelection.CategoryIndex) - 1));

	OnWidgetUpdated();
}

void UCampBuildingWidget::OnInputScrollItemPrevious()
{
	BuildMenuSelection.ItemIndex -= 1;
	BuildMenuSelection.ItemIndex = FMath::Clamp(BuildMenuSelection.ItemIndex, 0, (GetNumItemsInCategory(BuildMenuSelection.CategoryIndex) - 1));
	
	OnWidgetUpdated();
}

int32 UCampBuildingWidget::GetNumItemsInCategory(const int32 CategoryIndex) const
{
	if (!BuildableItemsArray.IsValidIndex(CategoryIndex))
	{
		return 0;
	}

	return BuildableItemsArray[CategoryIndex].Num();
}

void UCampBuildingWidget::UpdateSelectedItem()
{
	if (!IsValidItemIndex(BuildMenuSelection.CategoryIndex, BuildMenuSelection.ItemIndex))
	{
		BuildMenuSelection.SelectedItem = FBuildableItemTableRow();
		return;
	}

	BuildMenuSelection.SelectedItem = BuildableItemsArray[BuildMenuSelection.CategoryIndex][BuildMenuSelection.ItemIndex];

	// Update the camp building ability of the build menu selection change
	if (CampBuildingAbility.IsValid())
	{
		CampBuildingAbility->OnBuildMenuSelectionChange(BuildMenuSelection);
	}
}

bool UCampBuildingWidget::IsValidItemIndex(const int32 CategoryIndex, const int32 ItemIndex) const
{
	if (!BuildableItemsArray.IsValidIndex(CategoryIndex) || !BuildableItemsArray[CategoryIndex].IsValidIndex(ItemIndex))
	{
		return false;
	}

	return true;
}
