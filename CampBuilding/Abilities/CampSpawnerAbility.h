// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputActionValue.h"

// Custom includes

#include "CampSpawnerAbility.generated.h"

class ACampBase;
class ACampBuildingReticule;
class ACampSpawnTargetActor;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UAbilityTask_WaitTargetData;

UCLASS()
class TOKYOCOA_API UCampSpawnerAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UCampSpawnerAbility();	

protected:

	// Input context used for this ability.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CampSpawnerInputMappingContext = nullptr;

	// Input action for interacting in the build controls.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interact = nullptr;

	// Input action for rotating the spawn transform
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_RotateSpawn = nullptr;

	// Class used for the settlements
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CampBuilding")
	TSubclassOf<ACampBase> CampBaseClass = nullptr;

private:

	// Enhanced input component
	UEnhancedInputComponent* InputComponent = nullptr;
	
	// Enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;

	// A pointer to the wait target data class
	UAbilityTask_WaitTargetData* AT_WaitTargetData = nullptr;

	// The transform used to spawn the camp.
	FTransform CampSpawnTransform = FTransform();

	// A pointer to the target actor
	ACampSpawnTargetActor* SpawnedTargetActor = nullptr;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Test method to run a target data task
	void RunTask_WaitCampSpawnData();

	// Creates and sets up the input component.
	void SetupPlayerInput();

	// Spawns a camp at the spawn transform.
	void SpawnCampAtTransform();

	// Input recieved to interact
	void OnInputInteract();

	// Input received to rotate the spawn
	void OnInputRotateSpawn(const FInputActionValue &Value);

	UFUNCTION()
	void OnSpawnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);
};
