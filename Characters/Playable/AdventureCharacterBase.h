// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine classes
#include "CoreMinimal.h"

// Custom classes
#include "../HumanCharacterBase.h"
#include "../../PlayerHUD/GameHUDInterface.h"
#include "../../Inventory/InventoryInterface.h"
#include "AdventureCharacterInputInfo.h"
#include "AdventureCharacterAbilityInfo.h"

#include "AdventureCharacterBase.generated.h"

class ACampBase;
class UCameraComponent;
class APDAPawn;

// The main character class that will be used for the adventure mode
UCLASS()
class TOKYOCOA_API AAdventureCharacterBase : public AHumanCharacterBase, public IGameHUDInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:

	AAdventureCharacterBase();

	// Return a pointer to the core game hud
	UFUNCTION(BlueprintPure, Category = "Player HUD")
	virtual ACoreGameHUD* GetCoreGameHUD() override;

	// Return a pointer to the inventory component.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent();

	// Called by the settlement spawner when a new settlement is created 
	void OnCampCreated(TWeakObjectPtr<ACampBase> CreatedCamp);

	// Called by the settlement when it gets an overlap event from the player
	void OnCampBeginOverlap(TWeakObjectPtr<ACampBase> InEnteredSettlement);

	// Called by the settlement when the player leaves the settlement area
	void OnCampEndOverlap(TWeakObjectPtr<ACampBase> ExitedSettlement);

	// Returns the first person camera.
	TWeakObjectPtr<UCameraComponent> GetFirstPersonCamera() const;

	// Returns true if the pda is currently open.
	UFUNCTION(BlueprintCallable, Category = "PDA")
	bool GetIsPlayerPDAOpen() const;

	// Returns a pointer to the pda pawn child actor component.
	UFUNCTION(BlueprintPure, Category = "PDA")
	UChildActorComponent* GetPDAChildActorComponent() const;

	// Returns a pointer to the PDA pawn.
	UFUNCTION(BlueprintPure, Category = "PDA")
	APDAPawn* GetPDAPawn() const;

protected:

	// Input info related to this character.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FAdventureCharacterInputInfo InputInfo;

	// Data related to the ability system.
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FAdventureCharacterAbilityInfo AbilityInfo;

	// The first person camera of the character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdventureCharacter")
	UCameraComponent* FirstPersonCamera = nullptr;

	// The First person arms
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AdventureCharacter")
	USkeletalMeshComponent* FirstPersonArms = nullptr;

	// The PDA pawn child actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PDA")
	UChildActorComponent* PDAPawnActor = nullptr;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function to be called when damage received and health at zero
	virtual void OnZeroHealth() override;

private:



	// Input received to toggle the build controls
	void OnInputToggleBuildControls();

	// Input received to confirm the buildable item selection.
	void OnInputInteract();

	// Input received to toggle the inventory screen
	void OnInputToggleInventory();

	// Called when player character gets the OnZeroHealth call.
	void HandlePlayerDeath();
	

};