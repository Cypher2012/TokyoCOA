// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "CampSpawnerAbility.h"

// Engine includes
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

// Engine enhanced input
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

// Custom includes
#include "../TargetActors/CampSpawnTargetActor.h"
#include "../Reticles/CampSpawnTargetReticle.h"
#include "../CampBase.h"

UCampSpawnerAbility::UCampSpawnerAbility()
{
	// CampBuilder tag added to builer abilities so that they block each other from running at the same time.
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CampBuilder"));

	// Block other camp builder abilities and the player inventory
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CampBuilder"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Inventory.PlayerPDA"));

	// Tag required in order to build a camp
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CharacterCanSpawnCamp"));

	InputComponent = CreateDefaultSubobject<UEnhancedInputComponent>("InputComponent");
}

void UCampSpawnerAbility::OnInputInteract()
{
	if (!IsValid(SpawnedTargetActor) || !SpawnedTargetActor->GetTargetReticle().IsValid())
	{
		return;
	}

	// Check if the target is valid before confirming the targeting.
	if (!SpawnedTargetActor->GetTargetReticle()->IsTargetValid())
	{
		return;
	}	

	SpawnedTargetActor->ConfirmTargeting();
}

void UCampSpawnerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	RunTask_WaitCampSpawnData();

	SetupPlayerInput();
}

void UCampSpawnerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (IsValid(CampSpawnerInputMappingContext) && IsValid(Subsystem))
	{
		Subsystem->RemoveMappingContext(CampSpawnerInputMappingContext);
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(ASC))
	{
		ASC->SpawnedTargetActors.RemoveSingle(SpawnedTargetActor);
	}	

	AT_WaitTargetData->EndTask();
}

void UCampSpawnerAbility::RunTask_WaitCampSpawnData()
{
	// Spawn the target actor. This will be passed to the wait target data constructor.
	FActorSpawnParameters SpawnParams;
	SpawnedTargetActor = GetWorld()->SpawnActor<ACampSpawnTargetActor>(ACampSpawnTargetActor::StaticClass(), SpawnParams);
	if (!IsValid(SpawnedTargetActor))
	{
		return;
	}

	// Create and activate teh wait target data task
	AT_WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, "AT_WaitTargetData", EGameplayTargetingConfirmation::UserConfirmed, SpawnedTargetActor);
	if (!IsValid(AT_WaitTargetData))
	{
		return;
	}	
	AT_WaitTargetData->ReadyForActivation();
	AT_WaitTargetData->ValidData.AddDynamic(this, &UCampSpawnerAbility::OnSpawnTargetDataReady);
}

void UCampSpawnerAbility::SetupPlayerInput()
{
	if (!IsValid(CampSpawnerInputMappingContext) || !IsValid(InputComponent) || !IsValid(IA_Interact) || !IsValid(IA_RotateSpawn) )
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	// Add the mapping context.
	Subsystem->AddMappingContext(CampSpawnerInputMappingContext, 0);

	// Add the action bindings.
	InputComponent->BindAction(IA_Interact, ETriggerEvent::Completed, this, &UCampSpawnerAbility::OnInputInteract);	
	InputComponent->BindAction(IA_RotateSpawn, ETriggerEvent::Triggered, this, &UCampSpawnerAbility::OnInputRotateSpawn);
	
}

void UCampSpawnerAbility::SpawnCampAtTransform()
{
	if (!IsValid(CampBaseClass))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Spawn an instance of the camp class
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(CampBaseClass, CampSpawnTransform, SpawnParams);

	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCampSpawnerAbility::OnInputRotateSpawn(const FInputActionValue& Value)
{
	FRotator UpdatedRotation = FRotator(0, Value.GetMagnitude(), 0) + CampSpawnTransform.GetRotation().Rotator();
	CampSpawnTransform.SetRotation(UpdatedRotation.Quaternion());

	// Update the target reticle ghost mesh
	SpawnedTargetActor->GetTargetReticle()->RotateGhostMesh(Value.GetMagnitude());
}

void UCampSpawnerAbility::OnSpawnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{	
	if (Data.Data.Num() != 1 || !Data.Data.IsValidIndex(0))
	{
		return;
	}

	// Cast target data to single target hit type
	TSharedPtr<FGameplayAbilityTargetData> TargetData = Data.Data[0];
	FGameplayAbilityTargetData_SingleTargetHit* TargetHitResult = (FGameplayAbilityTargetData_SingleTargetHit*)(TargetData.Get());
	if (!TargetHitResult)
	{
		return;
	}
	
	// Use the target data to set the location of the camp spawn transform
	CampSpawnTransform.SetLocation(TargetHitResult->HitResult.ImpactPoint);

	SpawnCampAtTransform();
}
