// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_PlayerPDA.h"

// Engine includes
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Custom includes
#include "../Characters/Playable/AdventureCharacterBase.h"
#include "../PlayerHUD/CoreGameHUD.h"
#include "PDAPawn.h"

UGameplayAbility_PlayerPDA::UGameplayAbility_PlayerPDA()
{
	// This will be used to block this from other abilities.
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Inventory.PlayerPDA"));

	// The gameplay tag that both abilities have so that they block each other.
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("CampBuilding.CampBuilder"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Inventory.PlayerPDA"));
}

UInventoryComponent* UGameplayAbility_PlayerPDA::GetInventoryComponent()
{
	// Get owning actor and try to cast to adventure character
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = Cast<AAdventureCharacterBase>(GetActorInfo().OwnerActor);
	if (!PlayerCharacter.IsValid())
	{
		return nullptr;
	}
	return PlayerCharacter->GetInventoryComponent();
}

ACoreGameHUD* UGameplayAbility_PlayerPDA::GetCoreGameHUD()
{
	TWeakObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController.IsValid())
	{
		return nullptr;
	}
	return Cast<ACoreGameHUD>(PlayerController->GetHUD());
}

void UGameplayAbility_PlayerPDA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get owning actor and try to cast to adventure character
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = Cast<AAdventureCharacterBase>(GetActorInfo().OwnerActor);
	if (!PlayerCharacter.IsValid())
	{
		return;
	}

	TWeakObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	if (!PlayerController.IsValid())
	{
		return;
	}

	if (IsValid(GetCoreGameHUD()))
	{
		GetCoreGameHUD()->HideMainGameWidget();
	}

	// Block character movement.
	PlayerCharacter->GetCharacterMovement()->DisableMovement();

	// Set the view target as the PDA pawn.
	PlayerController->SetViewTargetWithBlend(Cast<AActor>(PlayerCharacter->GetPDAPawn()), CameraBlendTime);

	// Give control to the PDA.
	PlayerCharacter->GetPDAPawn()->EnablePDA();

	// Play sound effect.
	if (IsValid(PDAOpenCloseSoundCue))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PDAOpenCloseSoundCue);
	}

	// Start the timer before the widget is displayed.
	FTimerHandle PDAOpenFinishedTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PDAOpenFinishedTimerHandle, this, &UGameplayAbility_PlayerPDA::OnPDAOpeningFinished, CameraBlendTime, false);
}

void UGameplayAbility_PlayerPDA::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = Cast<AAdventureCharacterBase>(GetActorInfo().OwnerActor);
	if (!PlayerCharacter.IsValid())
	{
		return;
	}
	TWeakObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	if (!PlayerController.IsValid())
	{
		return;
	}

	// Set the view target back to the player character.
	PlayerController->SetViewTargetWithBlend(PlayerCharacter.Get(), CameraBlendTime);

	// Disable the PDA controls.
	PlayerCharacter->GetPDAPawn()->DisablePDA();

	// Reenable character movement.
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// Play sound effect.
	if (IsValid(PDAOpenCloseSoundCue))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PDAOpenCloseSoundCue);
	}

	if (IsValid(GetCoreGameHUD()))
	{
		GetCoreGameHUD()->DisplayMainGameWidget();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_PlayerPDA::OnPDAOpeningFinished()
{

}
