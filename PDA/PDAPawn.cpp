// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "PDAPawn.h"

// Engine includes
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

// Custom includes

APDAPawn::APDAPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	// Create the PDA static mesh
	PDABodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("PDAMesh");
	PDABodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PDABodyMesh->SetSimulatePhysics(false);
	PDABodyMesh->SetupAttachment(RootComponent);

	// Create the PDA screen mesh
	PDAScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>("PDAScreenMesh");
	PDAScreenMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PDAScreenMesh->SetSimulatePhysics(false);
	PDAScreenMesh->SetupAttachment(PDABodyMesh);

	// Create the PDA camera
	PDACamera = CreateDefaultSubobject<UCameraComponent>("PDACamera");
	PDACamera->SetupAttachment(RootComponent);
	PDACamera->SetRelativeLocation(FVector(-25.0f, 0, 0));

	// Create the PDA widget
	PDAWidget = CreateDefaultSubobject<UWidgetComponent>("PDAWidget");
	PDAWidget->SetupAttachment(RootComponent);
	PDAWidget->SetRelativeLocation(FVector(-200.0f, 0, 0));
	PDAWidget->SetVisibility(false, true);
}

void APDAPawn::BeginPlay()
{
	Super::BeginPlay();
		
	if (IsValid(PDAWidget))
	{
		PDAWidget->SetRelativeLocation(FVector(-20000.0f, 0, 0));
	}
}


void APDAPawn::EnablePDA()
{
	if (!IsValid(PDAWidget))
	{
		return;
	}

	PDAWidget->SetVisibility(true, true);

	SetActorTickEnabled(true);

	bIsEnabled = true;
}

void APDAPawn::DisablePDA()
{
	if (!IsValid(PDAWidget))
	{
		return;
	}

	PDAWidget->SetVisibility(false, true);

	SetActorTickEnabled(false);

	bIsEnabled = false;
}

bool APDAPawn::GetIsEnabled() const
{
	return bIsEnabled;
}

