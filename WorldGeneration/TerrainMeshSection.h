// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"

class UTerrainMeshManager;

// Parent tree - this/TerrainMeshManager/WorldGenerator/AdventurePlayGameMode - spawned and owned by the TerrainMeshManager.

// This class presumes that we will always create grids with even sides. It will not allow for uneven amount of sides!!
// It will help generate a grid of a fixed, known size. The size is calculated by the TileSize*TileCount
// The vertices will also be ordered in a way that we can access them in a predictable way using coordinates

class TOKYOCOA_API TerrainMeshSection
{

public:

	TerrainMeshSection(FRandomStream* InWorldRandomStream, const int32 GridVertexCount, const float GridVertexSpacing);

	friend class UTerrainMeshManager;
	
	int32 GetSectionIndex() const;
	TArray<FVector> GetVertices() const;
	TArray<FVector> GetNormals() const;
	TArray<int32> GetTriangles() const;
	TArray<FVector2D> GetUVs() const;
	float GetGridSize() const;
	int32 GetNumVertices() const;

protected:

	// Updats the vertex as the given index with the new position
	void UpdateVertexPosition(const int32 VertexIndex, const FVector NewPosition);

	// Will flatten the terrain out from the center point. 
	// It will flatten the entire radius and then smoothly transition out the mesh
	// CenterPoint - The center location of where we need to start flattening from
	// FloorHeight - The height at which we will bring the mesh floor to
	// FlattenRadius - The radius from CenterPoint that will be matched height
	// SmoothFalloff - The distance over which smoothing will transition the new flattened area to the rest of the terrain
	void FlattenArea(FVector CenterPoint, float FloorHeight, float FlattenRadius, float SmoothFalloff);

	// Set the section index so that it aligns with the actual mesh section index
	void SetSectionIndex(const int32 NewSectionIndex);

	// Randomises the height of the grid
	void RandomiseGridHeight(const float MeshHeightRange);

	// Smooths the height of the mesh
	void SmoothMeshHeight(const int32 NumIterations);

	// Generates basic UV tiling
	void GenerateUVs(float UTiling, float VTiling);

	// Translate the section mesh. This will not tell the procedural mesh to update!
	void TranslateSection(const FVector MeshTranslation);

	// Gets an array of pointers to the vertices
	void GetVertexPointers(TArray<FVector*>& OutVertexPointers);

private:


	// The index this mesh is associated with the actual mesh section in the terrain mesh
	int32 SectionIndex = 0;

	// The number of vertices in each axis. 
	int32 VertexCount = 0;

	// The amount of spacing between the vertices.
	float VertexSpacing = 0;

	// Vertices that make up the grid.
	TArray<FVector> GridVertices;

	// Normals that make up the grid.
	TArray<FVector> GridNormals;

	// Triangles that make up the grid. In triplets. 
	TArray<int32> GridTriangles;

	// UV's of the grid.
	TArray<FVector2D> GridUVs;

	// The world random stream.
	FRandomStream* WorldRandomStream = nullptr;

	// Generates the grid vertices based on the parameters and also generates default normals
	void CreateVertices();

	// Creates the triangles to make up the grid. 
	void CreateTriangles();

	// Primarily used by the smoothing method to find the neighbor vertices to smooth with
	void GetNeighborVertices(const int32 VertexIndex, TArray<int32> OutNeighborIndices);

	// Finds the index of all the vertices that are withing the radius of the passed center point, on the x and y plane
	// All height differences are ignored.
	// We basically just want to find vertices that fit with the X, Y radius
	// CenterPoint - The center point to search the radius from
	// Radius - The radius around the center point that will be checked for vertices
	// VertexIndices - The indices of all the vertices that were found to be withing the radius
	void VertexIndicesInFlatRadius(const FVector2D CenterPoint, const float Radius, TArray<int32>& VertexIndices);

	// Helper functions to translate between index and coordinates
	static int32 ConvertCoordinatesToIndex(FIntPoint Coordinates, int32 NumItems);
	static FIntPoint ConvertIndexToCoordinate(int32 Index, int32 NumItems);
};

