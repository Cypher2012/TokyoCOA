// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "CampBuildingTargetActor.h"

// Engine classes
#include "Camera/CameraComponent.h"

// Custom classes
#include "../Reticles/CampBuildingTargetReticle.h"
#include "../../Characters/Playable/AdventureCharacterBase.h"

ACampBuildingTargetActor::ACampBuildingTargetActor()
{
    bDebug = false;
    MaxRange = 750.0f;
    TraceProfile.Name = "NoCollision";
    bLastTraceWasGood = false;

   static ConstructorHelpers::FClassFinder<ACampBuildingTargetReticle>
    CampBuildingTargetReticleClass(TEXT("Class '/Game/CampBuilding/Reticles/BP_CampBuildingTargetReticle.BP_CampBuildingTargetReticle_C'"));

   if (IsValid(CampBuildingTargetReticleClass.Class))
   {
       ReticleClass = CampBuildingTargetReticleClass.Class;
   }
   else
   {
       ReticleClass = ACampBuildingTargetReticle::StaticClass();
   }
}

void ACampBuildingTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    if (!IsValid(Ability))
    {
        return;
    }

    PlayerCharacter = Cast<AAdventureCharacterBase>(OwningAbility->GetOwningActorFromActorInfo());
    if (PlayerCharacter.IsValid())
    {
        FirstPersonCamera = PlayerCharacter->GetFirstPersonCamera();
    }

    CampBuildingTargetReticle = Cast<ACampBuildingTargetReticle>(ReticleActor);

    CollisionParams.AddIgnoredActor(PlayerCharacter.Get());
}

TWeakObjectPtr<ACampBuildingTargetReticle> ACampBuildingTargetActor::GetTargetReticle() const
{
    return Cast<ACampBuildingTargetReticle>(ReticleActor);
}

FTransform ACampBuildingTargetActor::GetTargetReticuleTransform() const
{
       return ReticleActor->GetTransform();    
}

FHitResult ACampBuildingTargetActor::PerformTrace(AActor* InSourceActor)
{
    FHitResult ReturnHitResult = FHitResult();

    if (!FirstPersonCamera.IsValid() || !PlayerCharacter.IsValid() || !CampBuildingTargetReticle.IsValid())
    {
        return ReturnHitResult;
    }

    // Setup trace start and end
    FVector TraceStart = FirstPersonCamera->GetComponentLocation();
    FVector TraceEnd = FirstPersonCamera->GetComponentLocation() + FirstPersonCamera->GetForwardVector() * MaxRange;

    // Do the line trace.    
    GetWorld()->LineTraceSingleByProfile(ReturnHitResult, TraceStart, TraceEnd, "WorldGround", CollisionParams);

    if (ReturnHitResult.bBlockingHit)
    {  
        CampBuildingTargetReticle->OnTargetActorLocationUpdate(ReturnHitResult.ImpactPoint);
    }
    else
    {
        CampBuildingTargetReticle->OnTargetActorLocationUpdate(TraceEnd);
    }

    return ReturnHitResult;
}