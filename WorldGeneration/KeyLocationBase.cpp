// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyLocationBase.h"

// Engine classes
#include "Components/SphereComponent.h"

// Custom classes

AKeyLocationBase::AKeyLocationBase()
{
	LocationBounds = CreateDefaultSubobject<USphereComponent>("ClearArea");	
	LocationBounds->SetSphereRadius(500.0f, true);
	LocationBounds->bHiddenInGame = false;
	LocationBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	RootComponent = LocationBounds;
}

float AKeyLocationBase::GetLocationBoundsRadius() const
{
	if (IsValid(LocationBounds))
	{
		return LocationBounds->GetUnscaledSphereRadius();
	}
	else
	{
		return 500.0f;
	}	
}

void AKeyLocationBase::BeginPlay()
{
	Super::BeginPlay();
	
}
