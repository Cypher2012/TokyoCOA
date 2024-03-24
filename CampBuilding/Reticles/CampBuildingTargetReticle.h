// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityWorldReticle.h"

// Custom includes
#include "../Structs/MeshSocketPair.h"

#include "CampBuildingTargetReticle.generated.h"

class ABuildableItem;

// World reticule used for camp building items. This will have a representation mesh of the currently selected item
UCLASS()
class TOKYOCOA_API ACampBuildingTargetReticle : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()
	
public:

	ACampBuildingTargetReticle();

	// Returns bIsTargetValid
	bool IsTargetValid() const;

	// Updates the ghost static mesh with the display mesh of the current selected item
	void UpdateGhostStaticMesh(UStaticMesh* NewGhostStaticMesh);

	// Rotates the ghost static mesh by the given value
	void RotateGhostMesh(const float Value);

	// Called by the target actor when it has found a new location. This will be used to update the transform of this target reticle
	void OnTargetActorLocationUpdate(const FVector UpdatedLocation);

	// Returns the transform of the actual ghost mesh
	FTransform GetGhostStaticMeshTransform() const;

protected:

	// A static mesh to be a visual representation of the ghost of the buildable item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UStaticMeshComponent* GhostStaticMeshComponent = nullptr;

	// Material to apply when valid target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UMaterialInstance* ValidMaterial = nullptr;

	// Material to apply to reticle when invalid target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camp Building")
	UMaterialInstance* InvalidMaterial = nullptr;

private:

	// The amount of rotation that has been added by the player rotating
	FRotator PlayerAdjustedRotation = FRotator();

	// ==== Ghost Snapping ====

		// The distance from the ghost mesh to find suitable snap targets
	float SearchDistance = 750.0f;

	// The distance that a ghost connection point should be withing to trigger snapping.
	float ConnectionSnappingDistance = 150.0f;

	// Final transform after snapping has been applied
	FTransform PostSnapTransform = FTransform();

	// An array of all the connection sockets found on the ghost mesh.
	TArray<FMeshSocketPair> GhostConnectionSocketPairs;

	// An array of all the connection sockets found in the world, paired with their associated static mesh component.
	TArray<FMeshSocketPair> WorldConnectionSocketPairs;

	// Cached player controller
	TWeakObjectPtr<APlayerController> CachedPlayerController;

	// Starts the process of snapping the ghost mesh
	void SnapGhostMesh();

	// Gets all the buildable items that are within the search distance from the ghost 
	void GetWorldConnectionPoints();

	// Gets all the connection points for all the actors found by GetWorldConnectionPoints()
	void GetBuildableItemConnectionSockets(const ABuildableItem* BuildableItem, TArray<FMeshSocketPair>& InWorldConnectionSocketPairs);

	// Get all the connection points on the ghost mesh
	void GetGhostMeshConnectionPoints();

	// Find a suitable snap target
	bool FindConnectionSnapTarget();

	// Gets the world location of the socket in the MeshSocketPair.
	FVector GetSocketWorldLocation(const FMeshSocketPair MeshSocketPair);

	// Gets the distance of the world location in screen space to the center of the viewport
	float GetDistanceToViewportCenter(const FVector WorldLocation);

	// Returns a cached or newly found pointer to the player controller
	TWeakObjectPtr<APlayerController> GetPlayerController();

	// Get the dot product between the given socket and the first person camera.
	float GetSocketToCameraDotProduct(const FMeshSocketPair SocketPair);

	// Gets the viewport center in screen space.
	FVector2D GetViewportCenter();

	// Gets the screen position and direction in world space.
	void ScreenPositionToWorld(const FVector2D& ScreenPosition, FVector& WorldLocation, FVector& WorldDirection);

	// Gets the forward vector of the socket in world space.
	FVector GetSocketWorldForwardVector(const FMeshSocketPair& MeshSocketPair);

	// Called when a snap to connection has been found. This will initiate the actual snapping of the ghost mesh and spawn transform.
	void OnConnectionSnapTargetFound(const FMeshSocketPair& GhostSocketPair, const FMeshSocketPair& WorldSocketPair);

	// === End of Ghost Snapping ===

	// Validates the final transform of the ghost 
	void ValidateTargetLocation();

	// Updates the material of the ghost mesh depending on the valid target
	void UpdateValidMaterial();
};
