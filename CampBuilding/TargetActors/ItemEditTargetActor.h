// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"

#include "ItemEditTargetActor.generated.h"

class ABuildableItem;

// Target actor used for editing items when camp building
UCLASS()
class TOKYOCOA_API AItemEditTargetActor : public AGameplayAbilityTargetActor_SingleLineTrace
{
	GENERATED_BODY()

public:

	AItemEditTargetActor();

	// Get the currently selected item.
	TWeakObjectPtr<ABuildableItem> GetSelectedItem() const;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	// The material we will overlay to selected items
	UPROPERTY(EditDefaultsOnly, Category = "Camp Building")
	UMaterialInstance* SelectedOverlayMaterial = nullptr;

private:

	// A weak pointer to the player character
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = nullptr;

	// A weak pointer to the first person camera
	TWeakObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	// A weak pointer to the selected item
	TWeakObjectPtr<ABuildableItem> SelectedItem = nullptr;

	// Performs a trace from the player character first person camera
	FHitResult PerformTrace(AActor* InSourceActor) override;

	// Update the current selected item and adds the selection material overlay.
	void SetSelectedItem(TWeakObjectPtr<ABuildableItem> NewSelectedItem);
};
