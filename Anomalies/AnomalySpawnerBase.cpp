// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "AnomalySpawnerBase.h"

#include "AnomalyBase.h"

AAnomalySpawnerBase::AAnomalySpawnerBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAnomalySpawnerBase::NotifyAnomalyDestroyed(AAnomalyBase* DestroyedAnomaly)
{
	// Spawner notified that an anomaly has been destroyed, so we spawn a replacement.
	SpawnRandomAnomaly();
}

void AAnomalySpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMaxAnomalies();
}

void AAnomalySpawnerBase::SpawnMaxAnomalies()
{
	for (int32 SpawnCount = 0; SpawnCount < MaxWorldAnomalies; SpawnCount++)
	{
		SpawnRandomAnomaly();
	}
}

void AAnomalySpawnerBase::SpawnRandomAnomaly()
{
	// Validate that at least one anomaly class has been defined.
	int32 NumAnomalyClasses = AnomalyClasses.Num();
	if (NumAnomalyClasses <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnRandomAnomaly: Num AnomalyClasses was 0"));
		return;
	}

	// Get a random index and check that it is valid.
	int32 ClassIndex = FMath::RandRange(0, NumAnomalyClasses-1);
	if (!AnomalyClasses.IsValidIndex(ClassIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnRandomAnomaly: Class index was invalid! Index: %i"), ClassIndex);
		return;
	}

	// Get the class of anomaly that will be spawned.
	TSubclassOf<AAnomalyBase> AnomalyClass = AnomalyClasses[ClassIndex];

	// Get a random transform
	FTransform AnomalyTransform = GetRandomTransform();

	// Actually spawn the anomaly
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AAnomalyBase* NewAnomaly = GetWorld()->SpawnActor<AAnomalyBase>(AnomalyClass, AnomalyTransform, SpawnParams);
	if (IsValid(NewAnomaly))
	{
		NewAnomaly->InitializeAnomaly(this);
	}
}

FTransform AAnomalySpawnerBase::GetRandomTransform()
{
	FTransform NewTransform = FTransform();
	NewTransform.SetLocation(FVector(FMath::FRandRange(-5000.0f, 5000.0f), FMath::FRandRange(-5000.0f, 5000.0f), FMath::FRandRange(0.0f, 10.0f)));
	return NewTransform;
}

