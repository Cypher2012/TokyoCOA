// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"

// Engine classes
#include "ProceduralMeshComponent.h"
#include "AI/NavigationSystemBase.h"

// Custom classes
#include "TerrainMeshManager.h"
#include "KeyLocationBase.h"

AWorldGenerator::AWorldGenerator()
{
	// Setup the terrain mesh
	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	RootComponent = TerrainMesh;
	TerrainMesh->bUseAsyncCooking = true;
	
	// Setup the terrain manager
	TerrainMeshManager = CreateDefaultSubobject<UTerrainMeshManager>("TerrainMeshManager");	

	TerrainMesh->SetCollisionProfileName("WorldGround");
}

bool AWorldGenerator::IsValidCampSpawn(const float CampBoundsRadius, const FVector &SpawnLocation)
{
	if (!IsValid(TerrainMeshManager))
	{
		return false;
	}

	// check that the spawn location is within the terrain bounds taking in to consideration the radius of the camp
	FBox TerrainBounds = AdjustBoundsByRadius(TerrainMeshManager->GetWorldTerrainBounds(), CampBoundsRadius);

	// Check if the spawn location is within the bounds of the terrain
	if (!TerrainBounds.IsInside(SpawnLocation))
	{
		return false;
	}

	// Now check if the spawn location will be too close to any of the spawned key locations
	for (AKeyLocationBase* SpawnedKeyLocation : SpawnedKeyLocations)
	{
		// Simple sphere collision test. The distance between the camp and the key location, should be no less than the
		// total of both of their bounds radius.
		float Distance = FVector::Distance(SpawnedKeyLocation->GetActorLocation(), SpawnLocation);
		float RequiredMiniumDistance = CampBoundsRadius + SpawnedKeyLocation->GetLocationBoundsRadius();
		if (Distance < RequiredMiniumDistance)
		{
			return false;
		}
	}

	// The camp is within the terrain bounds and is not too close to any existing key locations
	return true;
}

void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

	WorldRandomStream = new FRandomStream(WorldRandomSeed);
	TerrainMeshManager->Initialize(TerrainMesh, WorldRandomStream);
	SpawnKeyLocations();

	// Update navmesh	TODO: Debug why this is not working!
	FNavigationSystem::UpdateComponentData(*TerrainMesh);

	//TerrainMesh->UpdateNavigationBounds();	
}

void AWorldGenerator::SpawnKeyLocations()
{
	SpawnedKeyLocations.Empty();

	for (TSubclassOf<AKeyLocationBase> KeyLocationClass : KeyLocationActorClasses)
	{
		// Setup the spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the actor
		AKeyLocationBase* NewLocationActor = GetWorld()->SpawnActor<AKeyLocationBase>(KeyLocationClass, FVector(), FRotator(), SpawnParams);
		SpawnedKeyLocations.Add(NewLocationActor);

		// Gets a valid spawn location that we can move this actor to
		FVector SpawnLocation;
		bool WasSucces;
		FindKeyLocationValidSpawnLocation(NewLocationActor, SpawnLocation, WasSucces);

		// Ask the TerrainMeshManager for the most suitable height at this location
		float SpawnHeight;
		TerrainMeshManager->GetSuitableSpawnHeight(SpawnLocation, SpawnHeight);
		SpawnLocation.Z = SpawnHeight;

		// Set the location and rotation for the actor
		NewLocationActor->SetActorLocationAndRotation(SpawnLocation, FRotator(0, WorldRandomStream->FRandRange(0, 359.9f), 0));
		
		// TTerrainMeshManager to flatten the terrain surface around the KeyLocation
		TerrainMeshManager->FlattenAreaAllSections(SpawnLocation, SpawnHeight, NewLocationActor->GetLocationBoundsRadius());
	}
}

void AWorldGenerator::FindKeyLocationValidSpawnLocation(TWeakObjectPtr<AKeyLocationBase> SpawnedActor, FVector& OutSpawnLocation, bool &WasSuccess)
{
	WasSuccess = false;
	bool SpawnLocationIsValid = false;
	int32 MAX_SEARCH_ITERATIONS = 100;
	int32 SearchIterations = 0;

	while (!SpawnLocationIsValid && SearchIterations < MAX_SEARCH_ITERATIONS)
	{
		// Set this to true. If this reamins true, then we will break out of this loop
		// as we have found a location that is valid
		SpawnLocationIsValid = true;		

		// Pick a random x,y and z location thats within the bounds of the current terrain
		// Check location is with suitable bounds of terrain taking in to account the spawned actors clear area
		FVector SpawnLocation;
		float ClearArea = SpawnedActor->GetLocationBoundsRadius();
		FBox TerrainBounds = TerrainMeshManager->GetWorldTerrainBounds();

		SpawnLocation.X = WorldRandomStream->RandRange(TerrainBounds.Min.X + ClearArea, TerrainBounds.Max.X - ClearArea);
		SpawnLocation.Y = WorldRandomStream->RandRange(TerrainBounds.Min.Y + ClearArea, TerrainBounds.Max.Y - ClearArea);
		SpawnLocation.Z = WorldRandomStream->FRandRange(TerrainBounds.Min.Z, TerrainBounds.Max.Z);

		// Then loop through all other location actors and see if we are far enough away from them
		// Now check distance from this actor to all the other actors clear area
		for (AKeyLocationBase* OtherLocationActor : SpawnedKeyLocations)
		{	
			if (SpawnedActor != OtherLocationActor)
			{
				// Get the total required clear area
				float TotalClearAreaRequired = ClearArea + OtherLocationActor->GetLocationBoundsRadius();

				// Get the distance between the spawned actor and the OtherLocation actor
				float SeperationDistance = FVector::Distance(SpawnLocation, OtherLocationActor->GetActorLocation());

				// If too close, that it is not a valid area and the loop will continue
				if (SeperationDistance < TotalClearAreaRequired)
				{
					SpawnLocationIsValid = false;
				}
			}			
		}

		// If SpawnLocationIsValid is still true, we can end the loop as we found a suitable place
		if (SpawnLocationIsValid)
		{
			WasSuccess = true;
			OutSpawnLocation = SpawnLocation;
			return;
		}

		SearchIterations++;
	}
}

FBox AWorldGenerator::AdjustBoundsByRadius(const FBox InBounds, const float Radius, const bool EffectZAxis)
{
	FBox NewBounds = InBounds;

	// Adjust box max
	NewBounds.Max.X = InBounds.Max.X - Radius;
	NewBounds.Max.Y = InBounds.Max.Y - Radius;

	// Adjust box min
	NewBounds.Min.X = InBounds.Min.X + Radius;
	NewBounds.Min.Y = InBounds.Min.Y + Radius;

	if (EffectZAxis)
	{
		NewBounds.Max.Z = InBounds.Max.Z - Radius;
		NewBounds.Min.Z = InBounds.Min.Z + Radius;
	}

	return NewBounds;
}
