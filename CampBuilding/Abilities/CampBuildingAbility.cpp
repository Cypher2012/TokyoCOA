// Fill out your copyright notice in the Description page of Project Settings.

#include "CampBuildingAbility.h"

// Engine includes
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

// Enhanced input
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

// Custom includes
#include "../../PlayerHUD/CoreGameHUD.h"
#include "../../PlayerHUD/CampBuilding/CampBuildingWidget.h"
#include "../BuildableItem.h"
#include "../TargetActors/CampBuildingTargetActor.h"
#include "../Reticles/CampBuildingTargetReticle.h"
#include "../TargetActors/ItemEditTargetActor.h"

UCampBuildingAbility::UCampBuildingAbility()
{
	// Add the CampBuilder tag to this so it can be blocked by other camp builder
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CampBuilder"));

	// The gameplay tag that both abilities have so that they block each other.
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CampBuilder"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Inventory.PlayerPDA"));

	// The tag used for when the character is in camp bounds
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CharacterInCampBounds"));

	InputComponent = CreateDefaultSubobject<UEnhancedInputComponent>("InputComponent");

	UpdateCampBuildWidgetBuildMode();
}

void UCampBuildingAbility::OnInputInteract()
{
	if (CurrentBuildMode == ECampBuildMode::ECampObjectMode)
	{
		TryBuildItemSelection();
	}

	else if (CurrentBuildMode == ECampBuildMode::ECampEditMode)
	{
		DestroySelectedItem();
	}	
}

void UCampBuildingAbility::OnBuildMenuSelectionChange(FBuildMenuSelection& NewMenuSelection)
{
	CurrentBuildMenuSelection = NewMenuSelection;

	UpdateTargetReticleStaticMesh();
}

void UCampBuildingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	RunTask_ItemBuildWaitTargetData();

	DisplayCampBuildWidget();

	SetupPlayerInput();	
}

void UCampBuildingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(CampBuildingInputMappingContext) && IsValid(Subsystem))
	{
		Subsystem->RemoveMappingContext(CampBuildingInputMappingContext);
	}

	RemoveCampBuildWidget();

	StopTask_ItemBuildWaitTargetData();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

ACoreGameHUD* UCampBuildingAbility::GetCoreGameHUD()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); 
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	return Cast<ACoreGameHUD>(PlayerController->GetHUD());
}

TWeakObjectPtr<UCampBuildingWidget> UCampBuildingAbility::GetCampBuildWidget()
{
	if (!IsValid(GetCoreGameHUD()))
	{
		return nullptr;
	}

	return GetCoreGameHUD()->GetCampBuildWidget();
}

void UCampBuildingAbility::RunTask_ItemBuildWaitTargetData()
{
	// Spawn the target actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CampBuildingTargetActor = GetWorld()->SpawnActor<ACampBuildingTargetActor>(ACampBuildingTargetActor::StaticClass(), SpawnParams);
	if (!IsValid(CampBuildingTargetActor))
	{
		return;
	}

	// Start the wait data task
	AT_ItemBuildWaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, "AT_WaitTargetData", EGameplayTargetingConfirmation::UserConfirmed, CampBuildingTargetActor);
	if (!IsValid(AT_ItemBuildWaitTargetData))
	{
		return;
	}

	// Setup the callback function for when the target data is valid
	AT_ItemBuildWaitTargetData->ReadyForActivation();

	// Update the static mesh to that of the current selected item
	UpdateTargetReticleStaticMesh();
}

void UCampBuildingAbility::StopTask_ItemBuildWaitTargetData()
{
	if (!IsValid(AT_ItemBuildWaitTargetData))
	{
		return;
	}

	AT_ItemBuildWaitTargetData->EndTask();
}

void UCampBuildingAbility::RunTask_ItemEditWaitTargetData()
{
	// Spawn the item edit target actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(ItemEditTargetActorClass))
	{
		ItemEditTargetActor = GetWorld()->SpawnActor<AItemEditTargetActor>(ItemEditTargetActorClass, SpawnParams);
	}
	else
	{
		ItemEditTargetActor = GetWorld()->SpawnActor<AItemEditTargetActor>(AItemEditTargetActor::StaticClass(), SpawnParams);
	}

	
	if (!IsValid(ItemEditTargetActor))
	{
		return;
	}

	// Start the wait target data task
	AT_ItemEditWaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, "AT_ItemEditWaitTargetData", EGameplayTargetingConfirmation::UserConfirmed, ItemEditTargetActor);
	if (!IsValid(AT_ItemEditWaitTargetData))
	{
		return;
	}
	AT_ItemEditWaitTargetData->ReadyForActivation();
}

void UCampBuildingAbility::StopTask_ItemEditWaitTargetData()
{
	if (!IsValid(AT_ItemEditWaitTargetData))
	{
		return;
	}
	AT_ItemEditWaitTargetData->EndTask();
}

void UCampBuildingAbility::SetupPlayerInput()
{
	if (!IsValid(CampBuildingInputMappingContext) && !IsValid(InputComponent) && !IsValid(IA_Interact))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInput: Input mapping context invalid or input component invalid!"));
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInput: Player controller invalid!"));
		return;
	}

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!IsValid(Subsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInput: Subsystem invalid!"));
		return;
	}

	// Add the mapping context.
	Subsystem->AddMappingContext(CampBuildingInputMappingContext, 0);

	// Add the action bindings.
	InputComponent->BindAction(IA_Interact, ETriggerEvent::Completed, this, &UCampBuildingAbility::OnInputInteract);
	InputComponent->BindAction(IA_RotateSpawn, ETriggerEvent::Triggered, this, &UCampBuildingAbility::OnInputRotateSpawn);
	InputComponent->BindAction(IA_ToggleBuildMode, ETriggerEvent::Completed, this, &UCampBuildingAbility::OnInputToggleBuildMode);
}

void UCampBuildingAbility::DisplayCampBuildWidget()
{
	if (IsValid(GetCoreGameHUD()))
	{
		GetCoreGameHUD()->DisplayCampBuildingWidget();
	}

	// Set a pointer to this ability to the camp build widget
	if (GetCampBuildWidget().IsValid())
	{
		GetCampBuildWidget()->SetCampBuildingAbility(this);
	}
}

void UCampBuildingAbility::RemoveCampBuildWidget()
{
	if (IsValid(GetCoreGameHUD()))
	{
		GetCoreGameHUD()->RemoveCampBuildingWidget();
	}
}

void UCampBuildingAbility::OnInputRotateSpawn(const FInputActionValue& Value)
{
	CampBuildingTargetActor->GetTargetReticle()->RotateGhostMesh(Value.GetMagnitude());
}

void UCampBuildingAbility::OnInputToggleBuildMode()
{
	// Switch build mode
	if (CurrentBuildMode == ECampBuildMode::ECampObjectMode)
	{
		CurrentBuildMode = ECampBuildMode::ECampEditMode;

		StopTask_ItemBuildWaitTargetData();
		RunTask_ItemEditWaitTargetData();
	}
	else
	{
		CurrentBuildMode = ECampBuildMode::ECampObjectMode;

		StopTask_ItemEditWaitTargetData();
		RunTask_ItemBuildWaitTargetData();
	}

	UpdateCampBuildWidgetBuildMode();
}

void UCampBuildingAbility::UpdateCampBuildWidgetBuildMode()
{
	if (!GetCampBuildWidget().IsValid())
	{
		return;
	}

	GetCampBuildWidget()->OnBuildModeChanged(CurrentBuildMode);
}

void UCampBuildingAbility::TryBuildItemSelection()
{
	if (!CampBuildingTargetActor->GetTargetReticle()->IsTargetValid())
	{
		return;
	}

	// Get the current transform of the target reticles ghost static mesh
	BuildItemSpawnTransform = CampBuildingTargetActor->GetTargetReticle()->GetGhostStaticMeshTransform();

	// Get the currently selected item from the build menu
	if (!GetCampBuildWidget().IsValid())
	{
		return;
	}
	FBuildMenuSelection MenuSelection = GetCampBuildWidget()->GetBuildMenuSelection();
	TSubclassOf<ABuildableItem> ItemClass = MenuSelection.SelectedItem.ItemClass;

	// Check if selected item is valid
	if (!IsValid(ItemClass))
	{
		return;
	}

	// Spawn actor of class at the transform
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Actually spawn the buildable item
	ABuildableItem* SpawnedActor = GetWorld()->SpawnActor<ABuildableItem>(CurrentBuildMenuSelection.SelectedItem.ItemClass, BuildItemSpawnTransform, SpawnParams);
}

void UCampBuildingAbility::DestroySelectedItem()
{
	if (!IsValid(ItemEditTargetActor))
	{
		return;
	}

	TWeakObjectPtr<ABuildableItem> BuildableItem = ItemEditTargetActor->GetSelectedItem();
	if (!BuildableItem.IsValid())
	{
		return;
	}

	BuildableItem->Destroy();
}

void UCampBuildingAbility::UpdateTargetReticleStaticMesh()
{
	if (!IsValid(CampBuildingTargetActor))
	{
		return;
	}

	// Get the target reticle
	TWeakObjectPtr<ACampBuildingTargetReticle> TargetReticle = CampBuildingTargetActor->GetTargetReticle();
	if (!TargetReticle.IsValid())
	{
		return;
	}

	// Get selected item class and create default object
	ABuildableItem* BuildItemDefaultObject = CurrentBuildMenuSelection.SelectedItem.ItemClass.GetDefaultObject();
	if (!IsValid(BuildItemDefaultObject))
	{
		return;
	}

	// Update the static mesh of the target reticle with the default display mesh of the currently selected item
	TargetReticle->UpdateGhostStaticMesh(BuildItemDefaultObject->GetStaticDisplayMesh()->GetStaticMesh());
}


