// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityWorldReticle.h"

#include "CampSpawnTargetReticle.generated.h"

// World reticle for camp spawning
UCLASS()
class TOKYOCOA_API ACampSpawnTargetReticle : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()
	
public:

	ACampSpawnTargetReticle();

	// Returns bIsTargetValid
	bool IsTargetValid() const;

	// Rotates the ghost static mesh by the given value
	void RotateGhostMesh(const float Value);

protected:

	// Static mesh for the ghost mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UStaticMeshComponent* GhostStaticMeshComponent = nullptr;

	// Material to apply when valid target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UMaterialInstance* ValidMaterial = nullptr;

	// Material to apply to reticle when invalid target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UMaterialInstance* InvalidMaterial = nullptr;

private:

};
