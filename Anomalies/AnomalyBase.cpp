// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "AnomalyBase.h"

// Engine classes

// Custom classes
#include "AnomalySpawnerBase.h"

AAnomalyBase::AAnomalyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAnomalyBase::InitializeAnomaly(TWeakObjectPtr<AAnomalySpawnerBase> InAnomalySpawner)
{
	AnomalySpawner = InAnomalySpawner;

	bIsAnomalyInitialized = true;
}

void AAnomalyBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(FMath::FRandRange(MinLifeSpan, MaxLifeSpan));
}

void AAnomalyBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (AnomalySpawner.IsValid() && EndPlayReason == EEndPlayReason::Destroyed)
	{
		AnomalySpawner->NotifyAnomalyDestroyed(this);
	}
}

