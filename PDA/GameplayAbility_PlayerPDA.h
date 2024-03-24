// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

// Custom includes
#include "../Inventory/InventoryInterface.h"
#include "../PlayerHUD/GameHUDInterface.h"

#include "GameplayAbility_PlayerPDA.generated.h"

class USoundCue;

// The ability that allows the player to open up the PDA
UCLASS()
class TOKYOCOA_API UGameplayAbility_PlayerPDA : public UGameplayAbility, public IInventoryInterface, public IGameHUDInterface
{
	GENERATED_BODY()

public:

	UGameplayAbility_PlayerPDA();

	// Return a pointer to the inventory component.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent() override;

	// Return the core game HUD
	UFUNCTION(BlueprintPure, Category = "Inventory")
	virtual ACoreGameHUD* GetCoreGameHUD() override;

protected:

	// The time to blend from first person to PDA camera.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PDA")
	float CameraBlendTime = 0.5f;

	// The sound cue used when opening the PDA.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	USoundCue* PDAOpenCloseSoundCue = nullptr;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	// This is manually called for when the PDA has finished opening and the camera has finished switching.
	void OnPDAOpeningFinished();
	
};
