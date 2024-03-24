// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "AdventurePlayGameMode.generated.h"

class AWorldGenerator;
class AAdventureCharacterBase;
class ACoreGameHUD;
class AAnomalySpawnerBase;

// Parent tree - this/ ROOT - This is essentially the root of everything in the game.

// The main game mode of the adventure game. It will also handle spawning and managing the
// world generator and player character.
UCLASS()
class TOKYOCOA_API AAdventurePlayGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:	

protected:

	// The world generator blueprint class. Set in editor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WorldGeneration")
	TSubclassOf<AWorldGenerator> WorldGeneratorClass = nullptr;

	// The character blueprint class that will be spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AdventureCharacter")	
	TSubclassOf<AAdventureCharacterBase> AdventureCharacterClass = nullptr;

	// The class to use to spawn an Anomaly Spawner actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anomaly")
	TSubclassOf<AAnomalySpawnerBase> AnomalySpawnerClass = nullptr;

	AAdventurePlayGameMode();

	virtual void BeginPlay() override;	

	virtual void Tick(float DeltaTime) override;

	// Returns a weak pointer to the core game HUD
	TWeakObjectPtr<ACoreGameHUD> GetCoreGameHUD() const;

private:

	// An instance of a WorldGenerator. This class is the spawner and owner of it.
	AWorldGenerator* WorldGenerator = nullptr;
	
	// An instance of AAdventureCharacterBase. This class is the spawner and owner of it.
	AAdventureCharacterBase* PlayerAdventureCharacter = nullptr;

	// A pointer to the anomaly spawner
	AAnomalySpawnerBase* AnomalySpawner = nullptr;

	// Spawns an instance of the world generator.
	void SpawnWorldGenerator();

	// Spawns an instance of the player character.
	void SpawnAdventureCharacter();

	// Spawns an instance of an anomaly spawner.
	void SpawnAnomalySpawner();

};
