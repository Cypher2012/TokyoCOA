// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

// Custom includes
#include "../../PlayerHUD/CampBuilding/CampBuildingWidget.h"
#include "../Enums/CampBuildMode.h"
#include "../../PlayerHUD/GameHUDInterface.h"

#include "CampBuildingAbility.generated.h"

class ACoreGameHUD;
class UCampBuildingWidget;
class AItemEditTargetActor;
class UInputAction;
class UInputMappingContext;

UCLASS()
class TOKYOCOA_API UCampBuildingAbility : public UGameplayAbility, public IGameHUDInterface
{
	GENERATED_BODY()

public:

	UCampBuildingAbility();

	// Should be called by the camp build widget when the selection changes.
	void OnBuildMenuSelectionChange(FBuildMenuSelection &NewMenuSelection);

protected:

	// Input context used for this ability.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CampBuildingInputMappingContext = nullptr;

	// Input action for interacting in the build controls.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interact = nullptr;

	// Input action to toggle current build mode
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleBuildMode = nullptr;

	// Input action for rotating the spawn rotation
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_RotateSpawn = nullptr;

	// Gets the core game hud
	UFUNCTION(BlueprintCallable, Category = "Player HUD")
	virtual ACoreGameHUD* GetCoreGameHUD();

	// Gets a pointer to the camp build widget
	TWeakObjectPtr<UCampBuildingWidget> GetCampBuildWidget();

	// The item edit target actor class to use.
	UPROPERTY(EditDefaultsOnly, Category = "Camp Building")
	TSubclassOf<AItemEditTargetActor> ItemEditTargetActorClass = nullptr;

private:

	// A pointer to the wait target data used for item building
	class UAbilityTask_WaitTargetData* AT_ItemBuildWaitTargetData = nullptr;

	// An instance of a camp building target actor
	class ACampBuildingTargetActor* CampBuildingTargetActor = nullptr;

	// A pointer to a wait target data task used for editing items.
	class UAbilityTask_WaitTargetData* AT_ItemEditWaitTargetData = nullptr;

	// An instance of an item edit target actor.
	class AItemEditTargetActor* ItemEditTargetActor = nullptr;

	// Enhanced input component
	class UEnhancedInputComponent* InputComponent = nullptr;

	class UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;

	// Enum to keep track of the current build mode
	ECampBuildMode CurrentBuildMode = ECampBuildMode::ECampObjectMode;

	// The transform used to spawn the camp.
	FTransform BuildItemSpawnTransform = FTransform();

	// Stores the current build menu selection. This will be updated by the camp builder widget
	FBuildMenuSelection CurrentBuildMenuSelection = FBuildMenuSelection();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Runs the item build wait target data
	void RunTask_ItemBuildWaitTargetData();

	// Stops the item build wait target data task
	void StopTask_ItemBuildWaitTargetData();

	// Runs the item edit wait target data task
	void RunTask_ItemEditWaitTargetData();

	// Stops the item edit wait target data task
	void StopTask_ItemEditWaitTargetData();

	// Setup the player input bindings
	void SetupPlayerInput();

	// Rquests the game HUD to show the build widget
	void DisplayCampBuildWidget();

	// Requests the game HUD to remove the camp build widget
	void RemoveCampBuildWidget();

	// Input received to interact
	void OnInputInteract();

	// Input received to rotate the spawn
	void OnInputRotateSpawn(const FInputActionValue& Value);

	// Toggle the build mode
	void OnInputToggleBuildMode();

	// Updates the camp build widget about the current build mode
	void UpdateCampBuildWidgetBuildMode();

	// Attempts to spawn the selected item class
	void TryBuildItemSelection();

	// Destroys the selected item, if any.
	void DestroySelectedItem();

	// Updates the target reticle with the current selected item's display mesh
	void UpdateTargetReticleStaticMesh();
};
