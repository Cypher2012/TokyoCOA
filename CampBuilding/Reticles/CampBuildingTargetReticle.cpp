// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "CampBuildingTargetReticle.h"

// Engine includes
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshSocket.h"

// Custom includes
#include "../BuildableItem.h"
#include "../CampBase.h"

ACampBuildingTargetReticle::ACampBuildingTargetReticle()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	GhostStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("GhostStaticMeshComponent");
	GhostStaticMeshComponent->SetCollisionProfileName("NoCollision");
	GhostStaticMeshComponent->SetupAttachment(RootComponent);

	bIsTargetValid = true;
}

bool ACampBuildingTargetReticle::IsTargetValid() const
{
	return bIsTargetValid;
}

void ACampBuildingTargetReticle::UpdateGhostStaticMesh(UStaticMesh* NewGhostStaticMesh)
{
	GhostStaticMeshComponent->SetStaticMesh(NewGhostStaticMesh);
}

void ACampBuildingTargetReticle::RotateGhostMesh(const float Value)
{
	PlayerAdjustedRotation.Add(0, Value, 0);
}

void ACampBuildingTargetReticle::OnTargetActorLocationUpdate(const FVector UpdatedLocation)
{
	// Set location of target reticle to the updated location
	SetActorLocation(UpdatedLocation);

	// Set the local rotation of the ghost static mesh
	GhostStaticMeshComponent->SetRelativeRotation(PlayerAdjustedRotation);

	// Run the snapping 
	SnapGhostMesh();

	// Finally, validate the position of the ghost mesh
	ValidateTargetLocation();
}

FTransform ACampBuildingTargetReticle::GetGhostStaticMeshTransform() const
{
	return GhostStaticMeshComponent->GetComponentTransform();
}

void ACampBuildingTargetReticle::SnapGhostMesh()
{
	// Get all the connection points that exists on buildable items already
	GetWorldConnectionPoints();

	// No connection points found so no point going any further!
	if (WorldConnectionSocketPairs.Num() <= 0)
	{
		return;
	}

	// Gets all the connection points on the ghost mesh
	GetGhostMeshConnectionPoints();

	// Finds a snap target, updates the ghost transform and then stores the PostSnapTransform
	FindConnectionSnapTarget();
}

void ACampBuildingTargetReticle::GetWorldConnectionPoints()
{
	WorldConnectionSocketPairs.Empty();

	TArray<AActor*> FoundBuildableItemActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildableItem::StaticClass(), FoundBuildableItemActors);

	for (AActor* BuildItem : FoundBuildableItemActors)
	{
		// Filter items that are within the search distance
		if (FVector::Distance(GetGhostStaticMeshTransform().GetLocation(), BuildItem->GetActorLocation()) <= SearchDistance + ConnectionSnappingDistance)
		{
			// Cast BuildItem to ABuiildableItem type as we add it to the array
			GetBuildableItemConnectionSockets(Cast<ABuildableItem>(BuildItem), WorldConnectionSocketPairs);
		}
	}
}

void ACampBuildingTargetReticle::GetBuildableItemConnectionSockets(const ABuildableItem* BuildableItem, TArray<FMeshSocketPair>& InWorldConnectionSocketPairs)
{
	if (!IsValid(BuildableItem))
	{
		return;
	}

	// Get the static mesh
	UStaticMeshComponent* DisplayMesh = BuildableItem->GetStaticDisplayMesh();

	// Get the sockets of the static mesh.
	TArray<FName> SocketNames = DisplayMesh->GetAllSocketNames();

	for (FName SocketName : SocketNames)
	{
		// Filter the sockets that start with "ConnectionPoint".
		FString SocketNameString = SocketName.ToString();
		if (SocketNameString.StartsWith("ConnectionPoint", ESearchCase::CaseSensitive))
		{
			// Add them to array of world connection points.
			InWorldConnectionSocketPairs.Add(FMeshSocketPair(DisplayMesh, DisplayMesh->GetSocketByName(SocketName)));
		}
	}
}

void ACampBuildingTargetReticle::GetGhostMeshConnectionPoints()
{
	GhostConnectionSocketPairs.Empty();

	// Get all the socket names
	TArray<FName> SocketNames = GhostStaticMeshComponent->GetAllSocketNames();

	// Loop through all the sockets
	for (FName SocketName : SocketNames)
	{
		// Filter all the ones that prefixed with "ConnectionPoint". If contains the prefix, add to SnapPointSocketNames array.
		FString SocketNameString = SocketName.ToString();
		if (SocketNameString.StartsWith("ConnectionPoint", ESearchCase::CaseSensitive))
		{
			const UStaticMeshSocket* StaticMeshSocket = GhostStaticMeshComponent->GetSocketByName(SocketName);
			GhostConnectionSocketPairs.Add(FMeshSocketPair(GhostStaticMeshComponent, StaticMeshSocket));
		}
	}
}

bool ACampBuildingTargetReticle::FindConnectionSnapTarget()
{
	// Initialize variables to keep track of the best pair and it's distance to the center of the viewport
	FMeshSocketPair BestGhostSocketPair = FMeshSocketPair();
	FMeshSocketPair BestWorldSocketPair = FMeshSocketPair();
	float BestDistanceToViewportCenter = TNumericLimits<float>::Max();
	bool bSnapTargetFound = false;

	// Loop through GhostConnectionPointsSockets
	for (FMeshSocketPair GhostConnectionSocketPair : GhostConnectionSocketPairs)
	{
		// Loop through all WorldConnectionPointSockets
		for (FMeshSocketPair CurrentWorldSocketPair : WorldConnectionSocketPairs)
		{
			// Get distance between the two sockets
			float DistanceToTarget = FVector::Dist(GetSocketWorldLocation(GhostConnectionSocketPair), GetSocketWorldLocation(CurrentWorldSocketPair));

			// Get the screen pace distance between the world socket and the viewport center.
			float DistanceToViewportCenter = GetDistanceToViewportCenter(GetSocketWorldLocation(CurrentWorldSocketPair));

			// We have likely found two snapping points that are in the exact same location
			// Find better snap point based on the direction the player is looking.
			if (DistanceToTarget < ConnectionSnappingDistance && FMath::IsNearlyEqual(DistanceToViewportCenter, BestDistanceToViewportCenter))
			{
				// Pick the connection point that is more aligned with the player camera.
				if (GetSocketToCameraDotProduct(CurrentWorldSocketPair) > GetSocketToCameraDotProduct(BestWorldSocketPair))
				{
					BestWorldSocketPair = CurrentWorldSocketPair;
					BestDistanceToViewportCenter = DistanceToViewportCenter;
				}
			}

			// If distance is < ConnectionSnappingDistance then we have found a suitable connection.
			else if (DistanceToTarget < ConnectionSnappingDistance && DistanceToViewportCenter < BestDistanceToViewportCenter)
			{
				BestGhostSocketPair = GhostConnectionSocketPair;
				BestWorldSocketPair = CurrentWorldSocketPair;
				BestDistanceToViewportCenter = DistanceToViewportCenter;
				bSnapTargetFound = true;
			}
		}
	}

	if (bSnapTargetFound)
	{
		OnConnectionSnapTargetFound(BestGhostSocketPair, BestWorldSocketPair);
	}

	return bSnapTargetFound;
}

FVector ACampBuildingTargetReticle::GetSocketWorldLocation(const FMeshSocketPair MeshSocketPair)
{
	FTransform SocketTransform;
	MeshSocketPair.Socket->GetSocketTransform(SocketTransform, MeshSocketPair.MeshComponent.Get());
	return SocketTransform.GetLocation();
}

float ACampBuildingTargetReticle::GetDistanceToViewportCenter(const FVector WorldLocation)
{
	if (!GetPlayerController().IsValid())
	{
		return 0.0f;
	}


	// Find the center of the viewport.
	int32 ScreenWidth, ScreenHeight;
	GetPlayerController()->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ViewportCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	// Get the screen pace distance between the world socket and the viewport center.
	FVector2D WorldSocketScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetPlayerController().Get(), WorldLocation, WorldSocketScreenPosition);
	return FVector2D::Distance(WorldSocketScreenPosition, ViewportCenter);
}

TWeakObjectPtr<APlayerController> ACampBuildingTargetReticle::GetPlayerController()
{
	if (CachedPlayerController.IsValid())
	{
		return CachedPlayerController;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	CachedPlayerController = PlayerController;
	return CachedPlayerController;
}

float ACampBuildingTargetReticle::GetSocketToCameraDotProduct(const FMeshSocketPair SocketPair)
{
	// Get the viewport center in screen space.
	FVector2D ViewportCenter = GetViewportCenter();

	// convert to world space
	FVector ViewLocation;
	FVector ViewDirection;
	ScreenPositionToWorld(ViewportCenter, ViewLocation, ViewDirection);

	// Get the forward vector of the last and current socket pair
	FVector LastSocketForwardVector = GetSocketWorldForwardVector(SocketPair);

	// Get the dot product of the two sockets, vs the camera 
	return FVector::DotProduct(ViewDirection.GetSafeNormal(), LastSocketForwardVector.GetSafeNormal());
}

FVector2D ACampBuildingTargetReticle::GetViewportCenter()
{
	if (!GetPlayerController().IsValid())
	{
		return FVector2D();
	}

	// Find the center of the viewport.
	int32 ScreenWidth, ScreenHeight;
	GetPlayerController()->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ViewportCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	return ViewportCenter;
}

void ACampBuildingTargetReticle::ScreenPositionToWorld(const FVector2D& ScreenPosition, FVector& WorldLocation, FVector& WorldDirection)
{
	if (!GetPlayerController().IsValid())
	{
		return;
	}

	GetPlayerController()->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection);
}

FVector ACampBuildingTargetReticle::GetSocketWorldForwardVector(const FMeshSocketPair& MeshSocketPair)
{
	if (!MeshSocketPair.MeshComponent.IsValid())
	{
		return FVector::ZeroVector;
	}

	// Get the sockets local foward vector.
	FVector LocalFowardVector = MeshSocketPair.Socket->RelativeRotation.RotateVector(FVector::ForwardVector);

	// Get the world transform of the mesh component
	FTransform WorldTransform = MeshSocketPair.MeshComponent->GetComponentTransform();

	return WorldTransform.TransformVector(LocalFowardVector);
}

void ACampBuildingTargetReticle::OnConnectionSnapTargetFound(const FMeshSocketPair& GhostSocketPair, const FMeshSocketPair& WorldSocketPair)
{	
	// Remove the player adjusted rotation
	GhostStaticMeshComponent->SetRelativeRotation(FRotator(0, 0, 0));

	// Get the socket transforms	
	FTransform GhostSocketTransform;
	GhostSocketPair.Socket->GetSocketTransform(GhostSocketTransform, GhostSocketPair.MeshComponent.Get());
	FTransform WorldSocketTransform;
	WorldSocketPair.Socket->GetSocketTransform(WorldSocketTransform, WorldSocketPair.MeshComponent.Get());

	// Get the forward vectors of both sockets
	FVector GhostSocketForwardVector = GhostSocketTransform.GetUnitAxis(EAxis::X);
	FVector WorldSocketForwardVector = WorldSocketTransform.GetUnitAxis(EAxis::X);

	// Flip the foward vector of the ghost socket, so they face each other. 
	GhostSocketForwardVector *= -1.0f;

	// Find the rotation to align them but only on the Yaw axis. Roll and Pitch of items will not be supported.
	FQuat RotationToAlign = FQuat::FindBetween(GhostSocketForwardVector, WorldSocketForwardVector);
	FQuat SnapRotation = FQuat(FRotator(0.0f, RotationToAlign.Rotator().Yaw, 0.0f));

	// Apply the rotation. Apply now before calculating translation.
	GhostStaticMeshComponent->SetRelativeRotation(SnapRotation);

	// Now calulate the translation needed after the rotation of the ghost mesh has been applied. 
	FVector SnapTranslation = GetSocketWorldLocation(WorldSocketPair) - GetSocketWorldLocation(GhostSocketPair);
	DrawDebugSphere(GetWorld(), SnapTranslation, 50.0f, 6, FColor::Yellow, false, -1, 0u, 5.0f);

	// Apply the snap translation to the mesh
	SetActorLocation(GetActorLocation() + SnapTranslation);
}

void ACampBuildingTargetReticle::ValidateTargetLocation()
{
	bIsTargetValid = false;

	if (!IsValid(GhostStaticMeshComponent))
	{
		return;
	}	

	// Get the world location of the ghost static mesh
	FVector GhostLocation = GhostStaticMeshComponent->GetComponentLocation();

	// Check if the ghost mesh is within the bounds of a camp 
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampBase::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		TWeakObjectPtr<ACampBase> CampActor = Cast<ACampBase>(FoundActor);
		if (CampActor.IsValid())
		{
			// Get bounds of camp
			float CampBoundsRadius = CampActor->GetCampBoundsRadius();

			// Get location of camp
			FVector CampLocation = CampActor->GetActorLocation();

			float Distance = FVector::Distance(GhostLocation, CampLocation);
			if (Distance < CampBoundsRadius)
			{
				bIsTargetValid = true;
			}
		}
		if (bIsTargetValid)
			break;
	}

	UpdateValidMaterial();
}

void ACampBuildingTargetReticle::UpdateValidMaterial()
{
	if (!IsValid(GhostStaticMeshComponent) || !IsValid(ValidMaterial) || !IsValid(InvalidMaterial))
	{
		return;
	}

	if (bIsTargetValid)
	{
		GhostStaticMeshComponent->SetMaterial(0, ValidMaterial);
	}
	else
	{
		GhostStaticMeshComponent->SetMaterial(0, InvalidMaterial);
	}
}
