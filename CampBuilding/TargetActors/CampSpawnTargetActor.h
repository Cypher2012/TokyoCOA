// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"

#include "CampSpawnTargetActor.generated.h"

class UArrowComponent;
class ACampSpawnTargetReticle;
class UCameraComponent;

// Target actor used for spawning camps.
UCLASS()
class TOKYOCOA_API ACampSpawnTargetActor : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()
	
public:

	ACampSpawnTargetActor();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	TWeakObjectPtr<ACampSpawnTargetReticle> GetTargetReticle() const;

protected:

private:

	// Collision params to use on the line trace
	FCollisionQueryParams CollisionParams = FCollisionQueryParams();

	// A weak pointer to the player character
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = nullptr;

	// A weak pointer to the first person camera
	TWeakObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	// Performs a trace from the player character first person camera
	FHitResult PerformTrace(AActor* InSourceActor) override;

	// Checks if the target location is valid
	void ValidateCampWorldLocation(bool &bIsValidLocation, FHitResult &CampLocation);
};
