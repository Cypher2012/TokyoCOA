// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "CampSpawnTargetReticle.h"

ACampSpawnTargetReticle::ACampSpawnTargetReticle()
{
	bIsTargetValid = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	GhostStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("GhostStaticMeshComponent");
	GhostStaticMeshComponent->SetCollisionProfileName("NoCollision");
	GhostStaticMeshComponent->SetupAttachment(RootComponent);
}

bool ACampSpawnTargetReticle::IsTargetValid() const
{
	return bIsTargetValid;

}

void ACampSpawnTargetReticle::RotateGhostMesh(const float Value)
{
	if (!IsValid(GhostStaticMeshComponent))
	{
		return;
	}

	GhostStaticMeshComponent->AddLocalRotation(FRotator(0, Value, 0));
}
