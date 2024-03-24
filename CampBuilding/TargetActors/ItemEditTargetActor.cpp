// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEditTargetActor.h"

// Engine classes
#include "Camera/CameraComponent.h"

// Custom classes 
#include "../../Characters/Playable/AdventureCharacterBase.h"
#include "../BuildableItem.h"

AItemEditTargetActor::AItemEditTargetActor()
{
    bDebug = false;
    MaxRange = 750.0f;
    TraceProfile.Name = "NoCollision";
}

TWeakObjectPtr<ABuildableItem> AItemEditTargetActor::GetSelectedItem() const
{
    return SelectedItem;
}

void AItemEditTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    if (!IsValid(Ability))
    {
        return;
    }

    // Cache the player character and the first person camera
    PlayerCharacter = Cast<AAdventureCharacterBase>(OwningAbility->GetOwningActorFromActorInfo());
    if (PlayerCharacter.IsValid())
    {
        FirstPersonCamera = PlayerCharacter->GetFirstPersonCamera();
    }    
}

void AItemEditTargetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Remove overlay material from the selected item.
    if (SelectedItem.IsValid())
    {
        SelectedItem->GetStaticDisplayMesh()->SetOverlayMaterial(nullptr);
    }

    Super::EndPlay(EndPlayReason);
}

FHitResult AItemEditTargetActor::PerformTrace(AActor* InSourceActor)
{
    FHitResult ReturnHitResult = FHitResult();

    if (!PlayerCharacter.IsValid() || !FirstPersonCamera.IsValid())
    {
        return ReturnHitResult;
    }

    // Setup trace start and end
    FVector TraceStart = FirstPersonCamera->GetComponentLocation();
    FVector TraceEnd = FirstPersonCamera->GetComponentLocation() + FirstPersonCamera->GetForwardVector() * MaxRange;    

    // Setup collision params
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(InSourceActor);
    Params.AddIgnoredActor(PlayerCharacter.Get());

    // Do the line trace.
    GetWorld()->LineTraceSingleByChannel(ReturnHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Camera, Params);

    // No hit, so no item to select
    if (!ReturnHitResult.bBlockingHit)
    {
        SetSelectedItem(nullptr);
        return ReturnHitResult;
    }

    // If there was a hit, was it a buildable item. If not, then we haven't selected an item
    TWeakObjectPtr<ABuildableItem> BuildableItem = Cast<ABuildableItem>(ReturnHitResult.GetActor());
    if (!BuildableItem.IsValid())
    {
        SetSelectedItem(nullptr);
        return ReturnHitResult;
    }

    // Hit item was a buildable item, so set SelectedItem
    SetSelectedItem(BuildableItem);
    return ReturnHitResult;
}

void AItemEditTargetActor::SetSelectedItem(TWeakObjectPtr<ABuildableItem> NewSelectedItem)
{
    // Remove overlay from previous selected item.
    if (SelectedItem.IsValid())
    {
        SelectedItem->GetStaticDisplayMesh()->SetOverlayMaterial(nullptr);
    }    

    // Now update the overlay material on the new selected item
    SelectedItem = NewSelectedItem;
    if (SelectedItem.IsValid() && IsValid(SelectedOverlayMaterial))
    {
        SelectedItem->GetStaticDisplayMesh()->SetOverlayMaterial(SelectedOverlayMaterial);
    }

    
}
