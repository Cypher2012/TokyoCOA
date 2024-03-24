// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainMeshSection.h"

// Engine classes
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Custom classes
#include "TerrainMeshManager.h"

// Public functions

TerrainMeshSection::TerrainMeshSection(FRandomStream* InWorldRandomStream, const int32 GridVertexCount, const float GridVertexSpacing)
{
	// We need VertexCount to be at least 2. 
	VertexCount = GridVertexCount;
	if (VertexCount < 2)
	{
		VertexCount = 2;
	}

	// We need to make sure VertexSpacing is a sane amount
	VertexSpacing = GridVertexSpacing;
	if (VertexSpacing < 10.0f)
	{
		VertexSpacing = 10.0f;
	}

	WorldRandomStream = InWorldRandomStream;

	CreateVertices();
	CreateTriangles();
}

int32 TerrainMeshSection::GetSectionIndex() const
{
	return SectionIndex;
}

TArray<FVector> TerrainMeshSection::GetVertices() const
{
	return GridVertices;
}

TArray<FVector> TerrainMeshSection::GetNormals() const
{
	return GridNormals;
}

TArray<int32> TerrainMeshSection::GetTriangles() const
{
	return GridTriangles;
}

TArray<FVector2D> TerrainMeshSection::GetUVs() const
{
	return GridUVs;
}

float TerrainMeshSection::GetGridSize() const
{
	return (VertexCount - 1) * VertexSpacing;
}

int32 TerrainMeshSection::GetNumVertices() const
{
	return GridVertices.Num();
}

void TerrainMeshSection::UpdateVertexPosition(const int32 VertexIndex, const FVector NewPosition)
{
	GridVertices[VertexIndex] = NewPosition;
}

void TerrainMeshSection::FlattenArea(FVector CenterPoint, float FloorHeight, float FlattenRadius, float SmoothFalloff)
{
	// Find all the vertices that need to be flattened. We just want to check the X and Y component. 
	// We want to ignore the height component as we want to make sure that vertices that are within the flat radius
	// are effected. If we only select vertices that are in a 3D radius, we might miss some vertices if there is a big
	// height variance

	// An array containing all the indices of vertices found that need to be flattened
	TArray<int32> VertexToFlattenIndices;
	FVector2D CenterPoint2D = FVector2D(CenterPoint.X, CenterPoint.Y);
	VertexIndicesInFlatRadius(CenterPoint2D, FlattenRadius, VertexToFlattenIndices);

	for (int32 Index : VertexToFlattenIndices)
	{
		GridVertices[Index].Z = FloorHeight;
	}

	// TODO: Smooth out the vertices surrounding to blend it with the current mesh
}

void TerrainMeshSection::SetSectionIndex(const int32 NewSectionIndex)
{
	SectionIndex = NewSectionIndex;
}

void TerrainMeshSection::RandomiseGridHeight(const float MeshHeightRange)
{
	for (int i = 0; i < GridVertices.Num(); i++)
	{
		float NewHeight = WorldRandomStream->FRandRange(0, MeshHeightRange);
		GridVertices[i].Z = NewHeight;
	}
}

void TerrainMeshSection::SmoothMeshHeight(const int32 NumIterations)
{
	TArray<FVector> SmoothedVertices = GridVertices;

	for (int32 Iteration = 0; Iteration < NumIterations; ++Iteration)
	{
		for (int32 VertexIndex = 0; VertexIndex < GridVertices.Num(); ++VertexIndex)
		{
			TArray<int32> NeighborIndices;
			GetNeighborVertices(VertexIndex, NeighborIndices);

			float AverageHeight = 0.0f;

			// Calculate the average height of the neighbors
			for (int32 NeighborIndex : NeighborIndices)
			{
				AverageHeight += GridVertices[NeighborIndex].Z;
			}

			if (NeighborIndices.Num() > 0)
			{
				AverageHeight /= NeighborIndices.Num();
				SmoothedVertices[VertexIndex].Z = AverageHeight;
			}
		}
	}

	// Update the grid vertices with the smoothed vertices
	GridVertices = SmoothedVertices;
}

void TerrainMeshSection::GenerateUVs(float UTiling, float VTiling)
{
	GridUVs.Empty();

	for (const FVector& Vertex : GridVertices)
	{
		// Map the X and Y coordinates to UVs with tiling
		float U = Vertex.X / GetGridSize() * UTiling;
		float V = Vertex.Y / GetGridSize() * VTiling;

		// Add the UV coordinates to the array
		GridUVs.Add(FVector2D(U, V));
	}
}

void TerrainMeshSection::TranslateSection(const FVector MeshTranslation)
{
	for (int Index = 0; Index < GridVertices.Num(); Index++)
	{
		FVector NewVector = GridVertices[Index] + MeshTranslation;
		GridVertices[Index] = NewVector;
	}
}

void TerrainMeshSection::GetVertexPointers(TArray<FVector*>& OutVertexPointers)
{
	// Ensure that GridVertices is not empty
	if (GridVertices.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridVertices is empty in FindEdgeVertexIndices."));
		return;
	}

	for (int32 Index = 0; Index < GridVertices.Num(); Index++)
	{
		OutVertexPointers.Add(&GridVertices[Index]);
	}
}

void TerrainMeshSection::CreateVertices()
{
	GridVertices.Empty();
	GridNormals.Empty();

	FVector GridCenter = FVector((VertexCount - 1) * VertexSpacing / 2, (VertexCount - 1) * VertexSpacing / 2, 0);

	for (int32 x = 0; x < VertexCount; x++)
	{
		for (int32 y = 0; y < VertexCount; y++)
		{
			float VertexX = x * VertexSpacing;
			float VertexY = y * VertexSpacing;

			FVector NewVertex = FVector(VertexX, VertexY, 0);
			NewVertex -= GridCenter;

			GridVertices.Add(NewVertex);

			FVector NewNormal = FVector(0, 0, 1);
			GridNormals.Add(NewNormal);
		}
	}

	GenerateUVs(10, 10);
}

void TerrainMeshSection::CreateTriangles()
{
	GridTriangles.Empty();

	for (int32 x = 0; x < VertexCount - 1; x++)
	{
		for (int32 y = 0; y < VertexCount - 1; y++)
		{
			int32 BottomLeft = ConvertCoordinatesToIndex(FIntPoint(x, y), VertexCount);			
			int32 BottomRight = ConvertCoordinatesToIndex(FIntPoint(x + 1, y), VertexCount);			
			int32 TopLeft = ConvertCoordinatesToIndex(FIntPoint(x, y + 1), VertexCount);			
			int32 TopRight = ConvertCoordinatesToIndex(FIntPoint(x + 1, y + 1), VertexCount);

			// Triangle 1
			GridTriangles.Add(BottomLeft);
			GridTriangles.Add(TopRight);
			GridTriangles.Add(TopLeft);

			// Triangle 2
			GridTriangles.Add(BottomLeft);
			GridTriangles.Add(BottomRight);
			GridTriangles.Add(TopRight);
		}
	}
}

void TerrainMeshSection::GetNeighborVertices(const int32 VertexIndex, TArray<int32> OutNeighborIndices)
{
	OutNeighborIndices.Empty();

	FIntPoint Coordinate = ConvertIndexToCoordinate(VertexIndex, VertexCount);

	// Define offsets for adjacent vertices
	const TArray<FIntPoint> Offsets = {
		FIntPoint(-1, 0), FIntPoint(1, 0),
		FIntPoint(0, -1), FIntPoint(0, 1)
	};

	for (const FIntPoint& Offset : Offsets)
	{
		FIntPoint NeighborCoordinate = Coordinate + Offset;

		int32 NeighborIndex = ConvertCoordinatesToIndex(NeighborCoordinate, VertexCount);

		if (NeighborIndex >= 0 && NeighborIndex < GridVertices.Num())
		{
			OutNeighborIndices.Add(NeighborIndex);
		}
	}
}

void TerrainMeshSection::VertexIndicesInFlatRadius(const FVector2D CenterPoint, const float Radius, TArray<int32>& VertexIndices)
{
	TArray<int32> FoundIndices;
	FoundIndices.Empty();

	for (int32 Index = 0; Index < GridVertices.Num(); Index++)
	{
		if (GridVertices.IsValidIndex(Index))
		{
			FVector2D Vertex2D = FVector2D(GridVertices[Index].X, GridVertices[Index].Y);

			float Distance = FVector2D::Distance(CenterPoint, Vertex2D);
			if (Distance <= Radius)
			{
				FoundIndices.Add(Index);
			}
		}
	}

	VertexIndices = FoundIndices;
}

int32 TerrainMeshSection::ConvertCoordinatesToIndex(FIntPoint Coordinates, int32 NumItems)
{
	return Coordinates.X + Coordinates.Y * NumItems;
}

FIntPoint TerrainMeshSection::ConvertIndexToCoordinate(int32 Index, int32 NumItems)
{
	int32 x = Index % NumItems;
	int32 y = Index / NumItems;

	return FIntPoint(x, y);
}
