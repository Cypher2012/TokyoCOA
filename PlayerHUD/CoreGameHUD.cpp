// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreGameHUD.h"

// Engine includes
#include "Kismet/GameplayStatics.h"

// Custom includes
#include "CampBuilding/CampBuildingWidget.h"
#include "MainGameHUD/MainGameWidget.h"
#include "../Characters/Playable/AdventureCharacterBase.h"

UInventoryComponent* ACoreGameHUD::GetInventoryComponent()
{
	// Get player character
	ACharacter* PlayerCharacter =  UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	// Cast to adventure character and get its inventory component.
	AAdventureCharacterBase* AdventureCharacter = Cast<AAdventureCharacterBase>(PlayerCharacter);
	if (!IsValid(AdventureCharacter))
	{
		return nullptr;
	}

	return AdventureCharacter->GetInventoryComponent();
}

UAbilitySystemComponent* ACoreGameHUD::GetAbilitySystemComponent() const
{
	// Get player character
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	// Cast to adventure character and get its inventory component.
	AAdventureCharacterBase* AdventureCharacter = Cast<AAdventureCharacterBase>(PlayerCharacter);
	if (!IsValid(AdventureCharacter))
	{
		return nullptr;
	}

	return AdventureCharacter->GetAbilitySystemComponent();
}

void ACoreGameHUD::OnInventoryUpdated()
{
	UE_LOG(LogTemp, Warning, TEXT("ACoreGameHUD::OnInventoryUpdated"));
}

void ACoreGameHUD::DisplayMainGameWidget()
{
	if (!IsValid(MainGameWidget))
	{
		return;
	}
	MainGameWidget->SetVisibility(ESlateVisibility::Visible);
}

void ACoreGameHUD::HideMainGameWidget()
{
	if (!IsValid(MainGameWidget))
	{
		return;
	}

	MainGameWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ACoreGameHUD::DisplayCampBuildingWidget()
{
	// Collapse the main game widget
	if (IsValid(MainGameWidget))
	{
		MainGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//Create new instance of a camp build widget and add it to the viewport
	if (!IsValid(CampBuildingWidgetClass))
	{
		return;
	}
	CampBuildingWidget = CreateWidget<UCampBuildingWidget>(GetWorld(), CampBuildingWidgetClass.Get());
	if (!IsValid(CampBuildingWidget))
	{
		return;
	}
	CampBuildingWidget->AddToViewport(0);
}

void ACoreGameHUD::RemoveCampBuildingWidget()
{
	// Destroy the settlement build widget
	if (IsValid(CampBuildingWidget))
	{
		CampBuildingWidget->RemoveFromParent();
		CampBuildingWidget = nullptr;
	}

	// Show the main game widget.
	if (IsValid(MainGameWidget))
	{
		MainGameWidget->SetVisibility(ESlateVisibility::Visible);
	}	
}

TWeakObjectPtr<UCampBuildingWidget> ACoreGameHUD::GetCampBuildWidget() const
{
	return CampBuildingWidget;
}





void ACoreGameHUD::ShowPlayerDeathWidget()
{
	RemoveAllGameWidgets();

	if (!IsValid(PlayerDeathWidgetClass))
	{
		return;
	}

	UUserWidget* PlayerDeathWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerDeathWidgetClass.Get());
	if (IsValid(PlayerDeathWidget))
	{
		PlayerDeathWidget->AddToViewport();
	}
}

void ACoreGameHUD::BeginPlay()
{
	CreateMainGameWidget();
}

void ACoreGameHUD::CreateMainGameWidget()
{
	// Check for a valid widget class. This should be set in the blueprint child game mode.
	if (!IsValid(MainGameWidgetClass))
	{
		return;
	}

	// Create the main game widget and add it to the viewport.
	MainGameWidget = CreateWidget<UMainGameWidget>(GetWorld(), MainGameWidgetClass.Get());
	if (!IsValid(MainGameWidget))
	{
		return;
	}
	MainGameWidget->AddToViewport(0);
	MainGameWidget->SetVisibility(ESlateVisibility::Visible);
}

void ACoreGameHUD::RemoveAllGameWidgets()
{
	if (IsValid(MainGameWidget))
	{
		MainGameWidget->RemoveFromParent();
	}
	if (IsValid(CampBuildingWidget))
	{
		CampBuildingWidget->RemoveFromParent();
	}
}