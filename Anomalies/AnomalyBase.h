// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Custom classes


#include "AnomalyBase.generated.h"

class AAnomalySpawnerBase;

// Base class for all anomalies. Sets the lifespan of the anomaly and handles their deletion
// They will also notify the anomaly spawner when they are destroyed.
UCLASS()
class TOKYOCOA_API AAnomalyBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AAnomalyBase();

	void InitializeAnomaly(TWeakObjectPtr<AAnomalySpawnerBase> InAnomalySpawner);

protected:

	// Life span of the anomoly.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anomaly")
	float MinLifeSpan = 100.0f;

	// Life span of the anomoly.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anomaly")
	float MaxLifeSpan = 1000.0f;

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:

	// If the anomaly has been explicity initialized.
	bool bIsAnomalyInitialized = false;

	// A weak pointer to the anomaly spawner.
	TWeakObjectPtr<AAnomalySpawnerBase> AnomalySpawner = nullptr;
};
