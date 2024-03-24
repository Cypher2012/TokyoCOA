// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "TerrainMeshManager.h"

// Engine classes
#include "ProceduralMeshComponent.h"
#include "AI/NavigationSystemBase.h"

// Custom classes
#include "TerrainMeshSection.h"

bool UTerrainMeshManager::GetIsInitialized() const
{
	return bIsInitialized;
}

FBox UTerrainMeshManager::GetWorldTerrainBounds() const
{
	float WorldSize = GetSectionSize() * TerrainSectionCount;
	FBox WorldBounds;
	WorldBounds.Max = FVector( (WorldSize/2), (WorldSize / 2), GridMeshHeightRange);
	WorldBounds.Min = FVector(-(WorldSize / 2), -(WorldSize / 2), 0);

	return WorldBounds;
}

float UTerrainMeshManager::GetSectionSize() const
{
	return GridVertexSpacing * (GridVertexCount - 1);
}

void UTerrainMeshManager::Initialize(TWeakObjectPtr<UProceduralMeshComponent> InTerrainMesh, FRandomStream* InWorldRandomStream)
{
	TerrainMesh = InTerrainMesh;
	WorldRandomStream = InWorldRandomStream;

	bIsInitialized = true;

	BuildTerrainMeshSections();
}

void UTerrainMeshManager::GetSuitableSpawnHeight(const FVector &SpawnLocation, float &SpawnHeight)
{
	float DistanceToSpawnLocation = FLT_MAX;
	FVector* ClosestVertex = nullptr;

	// Search for the closes vertex
	for (FVector* Vertex : VertexPointers)
	{
		// Distance to Spawn location
		float Distance = FVector::Distance(*Vertex, SpawnLocation);
		if (Distance < DistanceToSpawnLocation)
		{
			// If distance < DistanceToSpawnLocation set ClosestVerex to Vertex
			DistanceToSpawnLocation = Distance;
			ClosestVertex = Vertex;
		}		
	}

	// Set spawn height to the height of the ClosestVertex
	if (!ClosestVertex)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSuitableSpawnHeight: ClosestVertex was nullptr!"));
	}

	SpawnHeight = ClosestVertex->Z;
}

void UTerrainMeshManager::FlattenAreaAllSections(FVector CenterPoint, float FloorHeight, float FlattenRadius)
{
	for (TerrainMeshSection* MeshSection : TerrainMeshSections)
	{
		MeshSection->FlattenArea(CenterPoint, FloorHeight, FlattenRadius, FlattenTerrainSmoothFalloff);
	}

	StitchTerrainSections();
	UpdateProceduralMeshSections();
}

void UTerrainMeshManager::BuildTerrainMeshSections()
{
	if (!TerrainMesh.IsValid() || !bIsInitialized)
	{
		return;
	}

	MakeTerrainNumbersSane();
	
	TerrainMeshSections.Empty();

	const float SectionSize = (GridVertexCount - 1) * GridVertexSpacing;
	const float TerrainMeshTotalSize = SectionSize * TerrainSectionCount;	

	// The translation that will move the entire group of sections so that the resulting mesh 
	// has it's pivot in the center. We will apply this to all the sections
	const FVector GroupTranslation = FVector(-(TerrainMeshTotalSize / 2) + (SectionSize/2), -(TerrainMeshTotalSize / 2) + (SectionSize / 2), 0);

	for (int32 IndexX = 0; IndexX < TerrainSectionCount; IndexX++)
	{
		for (int32 IndexY = 0; IndexY < TerrainSectionCount; IndexY++)
		{
			// Create TerrainMeshSection
			TerrainMeshSection* NewTerrainSection = new TerrainMeshSection(WorldRandomStream, GridVertexCount, GridVertexSpacing);
			
			// Translate the section location based on the index and size of sections and then add that 
			// to the group translation to place the section in it's final location
			const FVector SectionTranslation = FVector(IndexX * SectionSize, IndexY * SectionSize, 0);			
			NewTerrainSection->TranslateSection(SectionTranslation + GroupTranslation);	
			
			// Set section index based on the x and y components 
			int32 SectionIndex = TerrainMeshSection::ConvertCoordinatesToIndex(FIntPoint(IndexX, IndexY), TerrainSectionCount);
			NewTerrainSection->SetSectionIndex(SectionIndex);

			TerrainMeshSections.Add(NewTerrainSection);
		}
	}	

	// Randomize the terrain height and then smooth it.
	RandomiseAndSmoothTerrainSectionHeight();

	// Stitch the edges of all the sections once we have modified everything.
	StitchTerrainSections();

	// Creates the actual mesh sections on the UProceduralMeshComponent
	UpdateProceduralMeshSections();
}

void UTerrainMeshManager::MakeTerrainNumbersSane()
{
	TerrainSectionCount = FMath::Clamp(TerrainSectionCount, MIN_TERRAIN_SECTION_COUNT, MAX_TERRAIN_SECTION_COUNT);
	GridVertexCount = FMath::Clamp(GridVertexCount, MIN_GRID_VERTEX_COUNT, MAX_GRID_VERTEX_COUNT);
	GridVertexSpacing = FMath::Clamp(GridVertexSpacing, MIN_VERTEX_SPACING, MAX_VERTEX_SPACING);

	UE_LOG(LogTemp, Warning, TEXT("UTerrainMeshManager:MakeTerrainNumbersSane..."));
}

void UTerrainMeshManager::StitchTerrainSections()
{
	StoreAllVerticesAsPointers();

	for (FVector* VertexPointerOuter : VertexPointers)
	{
		// An array to pointers of vectors that share the same x,y component
		TArray<FVector*> MatchingVertices;

		// Increment the height of this and all found vertices so we can then calculate the average of them all
		float TotalHeight = VertexPointerOuter->Z;

		// Find all vertices that share the same x/y location
		for (FVector* VertexPointerInner : VertexPointers)
		{
			bool IsNearlyEqualX = FMath::IsNearlyEqual(VertexPointerOuter->X, VertexPointerInner->X);
			bool IsNearlyEqualY = FMath::IsNearlyEqual(VertexPointerOuter->Y, VertexPointerInner->Y);

			if (IsNearlyEqualX && IsNearlyEqualY)
			{
				MatchingVertices.Add(VertexPointerInner);
				TotalHeight += VertexPointerInner->Z;
			}
		}

		float AverageHeight = TotalHeight / (1 + MatchingVertices.Num());

		// Set the height of all the vertices to the average height
		VertexPointerOuter->Z = AverageHeight;

		// Now loop through the matching vertices and set their height to match the average height
		for (FVector* MatchingVertex : MatchingVertices)
		{
			MatchingVertex->Z = AverageHeight;
		}
	}
}

void UTerrainMeshManager::StoreAllVerticesAsPointers()
{
	if (!bIsInitialized || TerrainMeshSections.Num() == 0)
	{
		return;
	}

	VertexPointers.Empty();	

	for (int32 Index = 0; Index < TerrainMeshSections.Num(); ++Index)
	{
		TArray<FVector*> TempVertexPointers;
		TerrainMeshSections[Index]->GetVertexPointers(TempVertexPointers);
		VertexPointers.Append(TempVertexPointers);
	}
}

void UTerrainMeshManager::UpdateProceduralMeshSections()
{
	if (!TerrainMesh.IsValid() || !bIsInitialized)
	{
		return;
	}

	for (TerrainMeshSection* MeshSection : TerrainMeshSections)
	{
		TerrainMesh->CreateMeshSection_LinearColor(
			MeshSection->GetSectionIndex(),
			MeshSection->GetVertices(),
			MeshSection->GetTriangles(),
			MeshSection->GetNormals(),
			MeshSection->GetUVs(),
			TArray<FLinearColor>(),
			TArray<FProcMeshTangent>(),
			true
		);
	}

	FNavigationSystem::UpdateComponentData(*TerrainMesh);
}

void UTerrainMeshManager::RandomiseAndSmoothTerrainSectionHeight()
{
	for (TerrainMeshSection* MeshSection : TerrainMeshSections)
	{
		MeshSection->RandomiseGridHeight(GridMeshHeightRange);
		MeshSection->SmoothMeshHeight(GridHeightSmoothingIterations);
	}
}
