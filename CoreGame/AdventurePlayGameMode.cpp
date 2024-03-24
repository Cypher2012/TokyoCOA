// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "AdventurePlayGameMode.h"

// Engine classes
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

// Custom classes
#include "../WorldGeneration/WorldGenerator.h"
#include "../Characters/Playable/AdventureCharacterBase.h"
#include "../PlayerHUD/CoreGameHUD.h"
#include "../Anomalies/AnomalySpawnerBase.h"

AAdventurePlayGameMode::AAdventurePlayGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AAdventurePlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnWorldGenerator();
	SpawnAdventureCharacter();
	SpawnAnomalySpawner();
}

void AAdventurePlayGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, "FrameTime: " + FString::SanitizeFloat(DeltaTime));
}

TWeakObjectPtr<ACoreGameHUD> AAdventurePlayGameMode::GetCoreGameHUD() const
{
	return Cast<ACoreGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
}

void AAdventurePlayGameMode::SpawnWorldGenerator()
{
	if (!IsValid(WorldGeneratorClass))
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WorldGenerator = GetWorld()->SpawnActor<AWorldGenerator>(WorldGeneratorClass.Get(), SpawnParams);
}

void AAdventurePlayGameMode::SpawnAdventureCharacter()
{

	// Get the player start location so that we can spawn the player at a suitable place
	AActor* GamePlayerStart;
	GamePlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	if (!IsValid(GamePlayerStart))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find player start!"));
	}

	FVector SpawnLocation = GamePlayerStart->GetActorLocation();
	FRotator SpawnRotation = GamePlayerStart->GetActorRotation();

	// Spawn player character and pass it a reference to the world generator
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (!IsValid(AdventureCharacterClass))
	{
		return;
	}

	PlayerAdventureCharacter = GetWorld()->SpawnActor<AAdventureCharacterBase>(AdventureCharacterClass.Get(), SpawnLocation, SpawnRotation, SpawnParams);
}

void AAdventurePlayGameMode::SpawnAnomalySpawner()
{
	if (!IsValid(AnomalySpawnerClass))
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AnomalySpawner = GetWorld()->SpawnActor<AAnomalySpawnerBase>(AnomalySpawnerClass.Get(), SpawnParams);
}
