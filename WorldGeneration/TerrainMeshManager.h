// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine classes
#include "CoreMinimal.h"

// Custom classes

#include "TerrainMeshManager.generated.h"

class UProceduralMeshComponent;
class TerrainMeshSection;

// Parent tree - this/WorldGenerator/AdventurePlayGameMode - Created and spawned by the world generator.

// A manager class that will manage all the terrain sections. It is a direct bridge
// to TerrainMeshSection class. an instance should be created and owned by the world 
// generator actor. Its the bridge between WorldGenerator and TerrainMeshSection.
UCLASS()
class TOKYOCOA_API UTerrainMeshManager : public UActorComponent
{
	GENERATED_BODY()

public:

	friend class AWorldGenerator;	

	// Returns true if the TerrainMeshManager has been initialised and the IsInitialised flag is set to true.
	UFUNCTION(BlueprintPure, Category = "WorldGeneration")
	bool GetIsInitialized() const;

	// Calculates and retuns a bounding box that the terrain is inside.
	UFUNCTION(BlueprintPure, Category = "WorldGeneration")
	FBox GetWorldTerrainBounds() const;	

	// Get the size of a section on one dimension.
	UFUNCTION(BlueprintPure, Category = "WorldGeneration")
	float GetSectionSize() const;

protected:

	// The number of sections that make up the overall world terrain mesh. Setting this to 2 will create a 2x2 grid of sections
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	int32 TerrainSectionCount = 1.0f;

	// The number of vertices making up the grid in one dimension. Setting this to 10 will make a 10x10 grid of vertices.
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	int32 GridVertexCount = 10.0f;

	// The amount of space between the grid vertices.
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	float GridVertexSpacing = 500.0f;

	// The maximum height of the mesh grids.
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	float GridMeshHeightRange = 50.0f;

	// How many iterations when smoothing terrain mesh.
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	int GridHeightSmoothingIterations = 2;

	// When flattening the terrain, how much falloff there should be when smoothing area around.
	UPROPERTY(EditDefaultsOnly, Category = "WorldGeneration")
	float FlattenTerrainSmoothFalloff = 250.0f;

	// Sets the UProceduralMeshComponent that is being used for the terrain 
	// Sets the shared FRandomStream used by the WorldGenerator, so they all use the same one.
	void Initialize(TWeakObjectPtr<UProceduralMeshComponent> InTerrainMesh, FRandomStream* InWorldRandomStream);

	// Takes the passed in Spawn location and finds a suitable height based on the terrain mesh
	// It does not update the passed in spawn location.
	void GetSuitableSpawnHeight(const FVector &SpawnLocation, float &SpawnHeight);

	// Goes through all the terrain sections and calls their FlattenArea method
	void FlattenAreaAllSections(FVector CenterPoint, float FloorHeight, float FlattenRadius);

private:

	// Whether of not this actor has been initialised.
	bool bIsInitialized = false;

	// A weak pointer to the terrain mesh. Should be owned by WorldGenerator.
	TWeakObjectPtr<UProceduralMeshComponent> TerrainMesh = nullptr;

	// A pointer to the world random stream.
	//FRandomStream* WorldRandomStream = nullptr;
	FRandomStream* WorldRandomStream = nullptr;

	// An array of mesh sections that this class has generated.
	TArray<TerrainMeshSection*> TerrainMeshSections;

	// An array of vertex pointers. Used for when we just do a blanket operation on all vertices 
	// without any regard to what section it's in.
	TArray<FVector*> VertexPointers;

	// Constants
	// Minimum number of sections. 
	const int32 MIN_TERRAIN_SECTION_COUNT = 1;

	// Max number of sections. This can grow as performance is tested.
	const int32 MAX_TERRAIN_SECTION_COUNT = 20;

	// Minimum vertex count. Setting this to less than two will cause many headache.
	const int32 MIN_GRID_VERTEX_COUNT = 2;

	// Maximum vertex count of each grid. This can increase as performance is tested.
	const int32 MAX_GRID_VERTEX_COUNT = 10000;

	// Minimum vertex spacing. This can be lower but not advisable. 
	const int32 MIN_VERTEX_SPACING = 10;

	// Max vertex spacing. This can be increased as performance is tested.
	const int32 MAX_VERTEX_SPACING = 10000;

	// Initially creates the TerrainMeshSections and creates the mesh sections on the actual terrain mesh.
	void BuildTerrainMeshSections();

	// Clamps all terrain generation numbers so they are sane values.
	void MakeTerrainNumbersSane();

	// Ensures all the edges of the terrain sections match so they are seamless
	// Make sure StoreAllVerticesAsPointers() has been called at least once before this 
	// and then UpdateMeshSections() after to propogate the changes.
	void StitchTerrainSections();

	// Stores a pointer to all of the vertices.
	void StoreAllVerticesAsPointers();

	// Updates the terrain mesh sections
	void UpdateProceduralMeshSections();

	// Loops through all the terrain sections and calls their random height and smooth functions.
	// Make sure to call UpdateMeshSections() after this!
	void RandomiseAndSmoothTerrainSectionHeight();
};
