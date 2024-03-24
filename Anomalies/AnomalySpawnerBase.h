// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Custom classes


#include "AnomalySpawnerBase.generated.h"

class AAnomalyBase;

// Base class of the blueprint anomaly spawner class. This will handle what anomaly classes can be used.
// It will randomly spawn anomalies up to MaxWorldAnomalies. It will receive notifications from the anomalies 
// when they are destroyed, so the spawner will know when to spawn a replacement.
UCLASS()
class TOKYOCOA_API AAnomalySpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AAnomalySpawnerBase();

	// Called by anomalies on their EndPlay to notify the spawner that an anomaly has been destroyed!
	void NotifyAnomalyDestroyed(AAnomalyBase* DestroyedAnomaly = nullptr);

protected:

	// Maximum number of anomalies that can exists in the world.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anomaly")
	int32 MaxWorldAnomalies = 5;

	// The anomaly class types that can be spawned.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anomaly")
	TArray<TSubclassOf<AAnomalyBase>> AnomalyClasses;

	virtual void BeginPlay() override;

private:

	// Spawns the maximum number of anomalies all at once.
	void SpawnMaxAnomalies();

	// Spawns an anomoly. Picks randomly from the classes in AnomalyClasses.
	void SpawnRandomAnomaly();

	// Randomises the given transform location and rotation
	FTransform GetRandomTransform();
};
