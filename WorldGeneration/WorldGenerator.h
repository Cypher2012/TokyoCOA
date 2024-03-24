// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "TerrainMeshSection.h"

#include "WorldGenerator.generated.h"

class UProceduralMeshComponent;
class UTerrainMeshManager;
class AKeyLocationBase;

// Parent tree - this/AdventurePlayGameMode - Spawned and owned by the world generator.

// The top level class that handles generating the world. This handles communication with the terrain mesh manager
// as also handles the spawning of key locations. It will also be responsible for managing any other functions or manager
// classes that are involved with the world generation.
UCLASS()
class TOKYOCOA_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldGenerator();

	// This will check if the spawn location of the camp is valid.
	// Check againts the bounds of the terrain and how close to spawned key locations.
	bool IsValidCampSpawn(const float CampBoundsRadius, const FVector &SpawnLocation);

protected:

	virtual void BeginPlay() override;	

	// An instance of a terrain mesh manager. This class spawns and owns it.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldGeneration")
	UTerrainMeshManager* TerrainMeshManager = nullptr;

	// Seed used for the random stream. The random stream will be used by all.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WorldGeneration")
	int32 WorldRandomSeed = 1000;

	// An array holding all the classes of Key Location that the world generator will place in to the world.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WorldGeneration")
	TArray<TSubclassOf<AKeyLocationBase>> KeyLocationActorClasses;	

private:

	// The procedural mesh that makes the terrain.
	UProceduralMeshComponent* TerrainMesh = nullptr;
		
	// The world random stream. Created and owned by this class.
	FRandomStream* WorldRandomStream = nullptr;

	// An array holding all the key locations that have been spawned.
	TArray<AKeyLocationBase*> SpawnedKeyLocations;
	
	// Goes through all the KeyLocationActorClasses and spawnes them randomly in the world
	// It will take in to account current terrain, but the TerrainMeshManager should update the 
	// terrain mesh to make it more suitable for the KeyLocation
	void SpawnKeyLocations();

	// Takes the newly spawned key location actor and finds a valid location, taking in to account 
	// the terrain bounds, its and other location clear areas.
	void FindKeyLocationValidSpawnLocation(TWeakObjectPtr<AKeyLocationBase> SpawnedActor, FVector &OutSpawnLocation, bool &WasSuccess);

	// Reduce the size of the bounds by the radius given on the X, Y axis if EffectZAxis set to false.
	// Will also reduce Z axis if EffectZAxis set to true
	 FBox AdjustBoundsByRadius(const FBox InBounds, const float Radius, const bool EffectZAxis = false);
};
