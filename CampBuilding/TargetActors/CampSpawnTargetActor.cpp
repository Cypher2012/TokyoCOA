// Fill out your copyright notice in the Description page of Project Settings.


#include "CampSpawnTargetActor.h"

// Engine includes
#include "Components/ArrowComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Custom includes
#include "../../Characters/Playable/AdventureCharacterBase.h"
#include "../Reticles/CampSpawnTargetReticle.h"
#include "../../CampBuilding/CampBase.h"
#include "../../WorldGeneration/KeyLocationBase.h"

ACampSpawnTargetActor::ACampSpawnTargetActor()
{
	bDebug = false;
    MaxRange = 1500.0f;
    TraceProfile.Name = "NoCollision";

    static ConstructorHelpers::FClassFinder<ACampSpawnTargetReticle>
        CampSpawnTargetReticleClass(TEXT("Class '/Game/CampBuilding/Reticles/BP_CampSpawnTargetReticle.BP_CampSpawnTargetReticle_C'"));

    if (IsValid(CampSpawnTargetReticleClass.Class))
    {
        ReticleClass = CampSpawnTargetReticleClass.Class;
    }
    else
    {
        ReticleClass = ACampSpawnTargetReticle::StaticClass();
    }    
}

void ACampSpawnTargetActor::StartTargeting(UGameplayAbility* Ability)
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

    CollisionParams.AddIgnoredActor(PlayerCharacter.Get());
}

TWeakObjectPtr<ACampSpawnTargetReticle> ACampSpawnTargetActor::GetTargetReticle() const
{
    return Cast<ACampSpawnTargetReticle>(ReticleActor);
}

FHitResult ACampSpawnTargetActor::PerformTrace(AActor* InSourceActor)
{
    bLastTraceWasGood = false;
    FHitResult ReturnHitResult = FHitResult();
    FVector FirstTraceStart = FVector();    
    FVector FirstTraceEnd = FVector();

    if (!PlayerCharacter.IsValid())
    {
        return ReturnHitResult;
    }

    // Setup first line trace that finds a point in front of the player
    FirstTraceStart = PlayerCharacter->GetFirstPersonCamera()->GetComponentLocation();    
    AimWithPlayerController(InSourceActor, CollisionParams, FirstTraceStart, FirstTraceEnd);
    
    //Use a line trace initially to see where the player is actually pointing
    GetWorld()->LineTraceSingleByProfile(ReturnHitResult, FirstTraceStart, FirstTraceEnd, "WorldGround", CollisionParams);

    if (!ReturnHitResult.bBlockingHit)
    {
        ReturnHitResult.Location = FirstTraceEnd;
    }
       
    // Setup second trace directly down to find point on ground.
    FVector SecondTraceStart = ReturnHitResult.Location - (FirstTraceEnd - FirstTraceStart).GetSafeNormal();
    FVector SecondTraceEnd = SecondTraceStart;
    SecondTraceStart.Z += CollisionHeightOffset;
    SecondTraceEnd.Z -= 99999.0f;

    GetWorld()->LineTraceSingleByProfile(ReturnHitResult, SecondTraceStart, SecondTraceEnd, "WorldGround", CollisionParams);

    // If second trace didnt have any hits, use the first trace end so that we show target in front of player
    if (!ReturnHitResult.bBlockingHit)
    {
        ReturnHitResult.Location = FirstTraceEnd;
    }

    // Final check to see if location is within the bounds of another camp or key location
    ValidateCampWorldLocation(bLastTraceWasGood, ReturnHitResult); 

    // Update the target reticule
    if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
    {
        LocalReticleActor->SetIsTargetValid(bLastTraceWasGood);
        LocalReticleActor->SetActorLocation(ReturnHitResult.Location);
    }

    // Reset the trace start so the target data uses the correct origin
    ReturnHitResult.TraceStart = FirstTraceStart;

    return ReturnHitResult;
}

void ACampSpawnTargetActor::ValidateCampWorldLocation(bool& bIsValidLocation, FHitResult& CampHitResult)
{
    if (!CampHitResult.bBlockingHit)
    {
        bIsValidLocation = false;
        return;
    }  

    // Get default camp bounds as we will use this for both camps and key locations
    ACampBase* CampDefaultActor = Cast<ACampBase>(ACampBase::StaticClass()->GetDefaultObject(true));
    float CampBounds = CampDefaultActor->GetCampBoundsRadius(); // TODO: This will not be correct if camp bounds updated in blueprints

    // Get all camp and key location actors so we can get their location and bounds
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampBase::StaticClass(), FoundActors);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyLocationBase::StaticClass(), FoundActors);

    for (AActor* FoundActor : FoundActors)
    {        
        float Distance = FVector::Distance(FoundActor->GetActorLocation(), CampHitResult.Location);

        // Check if camp actor
        ACampBase* CampActor = Cast<ACampBase>(FoundActor);
        if (IsValid(CampActor))
        {
            CampBounds = CampActor->GetCampBoundsRadius(); // This will correct camp bounds, but only if there actually is a camp to get this value            
            if (Distance < CampBounds * 2)
            {
                bIsValidLocation = false;
                return;
            }
        }

        // Check if key location actor
        AKeyLocationBase* KeyLocationBase = Cast<AKeyLocationBase>(FoundActor);
        if (IsValid(KeyLocationBase))
        {
            float BoundsRadius = KeyLocationBase->GetLocationBoundsRadius();
            if (Distance < BoundsRadius + CampBounds)
            {
                bIsValidLocation = false;
                return;
            }
        }        
    }

    bIsValidLocation = true;
}
