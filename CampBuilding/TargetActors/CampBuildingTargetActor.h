// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "CampBuildingTargetActor.generated.h"

class ACampBuildingTargetReticle;
class AAdventureCharacterBase;
class UCameraComponent;

// Target actor used for the camp building ability. This will be responsible for tracing from the first person camera
UCLASS()
class TOKYOCOA_API ACampBuildingTargetActor : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()
	
public:

	ACampBuildingTargetActor();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	TWeakObjectPtr<ACampBuildingTargetReticle> GetTargetReticle() const;

	// Broadcast the current transform of the target transform. Call this when we want to do a check without killing the target actor
	FTransform GetTargetReticuleTransform() const;

protected:

private:

	// Collision params to use on the line trace
	FCollisionQueryParams CollisionParams = FCollisionQueryParams();

	// A weak pointer to the player character
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = nullptr;

	// A weak pointer to the first person camera
	TWeakObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	// A weaok pointer to the camp building target reticle
	TWeakObjectPtr<ACampBuildingTargetReticle> CampBuildingTargetReticle = nullptr;

	// Performs a trace from the player character first person camera
	FHitResult PerformTrace(AActor* InSourceActor) override;
};
