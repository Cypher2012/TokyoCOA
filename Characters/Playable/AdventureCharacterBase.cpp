// Fill out your copyright notice in the Description page of Project Settings.

#include "AdventureCharacterBase.h"

// Engine.General classes
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Engine.Input classes
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Custom.General classes
#include "../../WorldGeneration/WorldGenerator.h"
#include "../../PlayerHUD/CoreGameHUD.h"
#include "../../PDA/PDAPawn.h"

// Custom.CampBuilding
#include "../../CampBuilding/CampBase.h"
#include "../../CampBuilding/Abilities/CampBuildingAbility.h"
#include "../../CampBuilding/Abilities/CampSpawnerAbility.h"


AAdventureCharacterBase::AAdventureCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(-10, 0, 60));
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonArms");
	FirstPersonArms->SetupAttachment(FirstPersonCamera);

	PDAPawnActor = CreateDefaultSubobject<UChildActorComponent>("PDAPawnActor");
	PDAPawnActor->SetupAttachment(FirstPersonArms, "PDASocket");
}

ACoreGameHUD* AAdventureCharacterBase::GetCoreGameHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	return Cast<ACoreGameHUD>(PlayerController->GetHUD());
}

UInventoryComponent* AAdventureCharacterBase::GetInventoryComponent()
{
	return InventoryComponent;
}

void AAdventureCharacterBase::OnCampCreated(TWeakObjectPtr<ACampBase> CreatedCamp)
{
	OnCampBeginOverlap(CreatedCamp);

	if (!IsValid(ASC))
	{
		return;
	}

	ASC->CancelAbilityHandle(AbilityInfo.CampSpawnerAbilityHandle);
	ASC->TryActivateAbility(AbilityInfo.CampBuilderAbilityHandle);
}

void AAdventureCharacterBase::OnCampBeginOverlap(TWeakObjectPtr<ACampBase> NewSettlement)
{
	if (!IsValid(ASC))
	{
		return;
	}

	ASC->CancelAbilityHandle(AbilityInfo.CampSpawnerAbilityHandle);

	ASC->SetLooseGameplayTagCount(AbilityInfo.CharacterInCampBoundsTag, true);
	ASC->SetLooseGameplayTagCount(AbilityInfo.CharacterCanSpawnCampTag, false);
}

void AAdventureCharacterBase::OnCampEndOverlap(TWeakObjectPtr<ACampBase> ExitedSettlement)
{
	if (!IsValid(ASC))
	{
		return;
	}

	ASC->CancelAbilityHandle(AbilityInfo.CampBuilderAbilityHandle);

	// Update tags
	ASC->SetLooseGameplayTagCount(AbilityInfo.CharacterCanSpawnCampTag, true);
	ASC->SetLooseGameplayTagCount(AbilityInfo.CharacterInCampBoundsTag, false);

	if (!IsValid(GetCoreGameHUD()))
	{
		return;
	}
}

TWeakObjectPtr<UCameraComponent> AAdventureCharacterBase::GetFirstPersonCamera() const
{
	return FirstPersonCamera;
}

bool AAdventureCharacterBase::GetIsPlayerPDAOpen() const
{
	if (!IsValid(ASC))
	{
		return false;
	}

	FGameplayAbilitySpec* PlayerPDAABilitySpec = ASC->FindAbilitySpecFromHandle(AbilityInfo.PlayerPDAAbilityHandle);
	if (!PlayerPDAABilitySpec)
	{
		return false;
	}

	return PlayerPDAABilitySpec->IsActive();
}

UChildActorComponent* AAdventureCharacterBase::GetPDAChildActorComponent() const
{
	return PDAPawnActor;
}

APDAPawn* AAdventureCharacterBase::GetPDAPawn() const
{
	return Cast<APDAPawn>(PDAPawnActor->GetChildActor());
}

void AAdventureCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputInfo.BuildingMappingContext, 0);
	}

	if (!IsValid(ASC) || !IsValid(AbilityInfo.CampSpawnerAbilityClass) || !IsValid(AbilityInfo.CampBuildingAbilityClass) || !IsValid(AbilityInfo.PlayerPDAAbilityClass))
	{
		return;
	}

	ASC->SetLooseGameplayTagCount(AbilityInfo.CharacterCanSpawnCampTag, true);

	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.CampSpawnerAbilityClass);
	AbilityInfo.CampSpawnerAbilityHandle = ASC->GiveAbility(AbilitySpec);

	AbilitySpec = FGameplayAbilitySpec(AbilityInfo.CampBuildingAbilityClass);
	AbilityInfo.CampBuilderAbilityHandle = ASC->GiveAbility(AbilitySpec);

	AbilitySpec = FGameplayAbilitySpec(AbilityInfo.PlayerPDAAbilityClass);
	AbilityInfo.PlayerPDAAbilityHandle = ASC->GiveAbility(AbilitySpec);

	ASC->InitAbilityActorInfo(this, this);
	ASC->RefreshAbilityActorInfo();
}

void AAdventureCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputInfo.IA_ToggleBuildControls, ETriggerEvent::Completed, this, &AAdventureCharacterBase::OnInputToggleBuildControls);
		EnhancedInputComponent->BindAction(InputInfo.IA_ToggleShowInventory, ETriggerEvent::Completed, this, &AAdventureCharacterBase::OnInputToggleInventory);
	}
}

void AAdventureCharacterBase::OnZeroHealth()
{
	Super::OnZeroHealth();

	HandlePlayerDeath();
}

void AAdventureCharacterBase::OnInputToggleBuildControls()
{
	if (!IsValid(ASC))
	{
		return;
	}

	if (!ASC->TryActivateAbility(AbilityInfo.CampBuilderAbilityHandle, false) && !ASC->TryActivateAbility(AbilityInfo.CampSpawnerAbilityHandle, false))
	{
		ASC->CancelAbilityHandle(AbilityInfo.CampBuilderAbilityHandle);
		ASC->CancelAbilityHandle(AbilityInfo.CampSpawnerAbilityHandle);
	}
}

void AAdventureCharacterBase::OnInputToggleInventory()
{
	if (!IsValid(ASC))
	{
		return;
	}

	if (!ASC->TryActivateAbility(AbilityInfo.PlayerPDAAbilityHandle, false))
	{
		ASC->CancelAbilityHandle(AbilityInfo.PlayerPDAAbilityHandle);
	}
}

void AAdventureCharacterBase::HandlePlayerDeath()
{
	// Pause game
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->Pause();
	}

	// Request CoreGameHUD to show death menu
	if (!IsValid(GetCoreGameHUD()))
	{
		return;
	}
	GetCoreGameHUD()->ShowPlayerDeathWidget();
}






