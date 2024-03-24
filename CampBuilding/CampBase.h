// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CampBase.generated.h"

class USphereComponent;
class AAdventureCharacterBase;

// Parent tree - this/ no owners but is spawned by SettlementSpawnerComponent which is owned by AdventureCharacterBase

// The base class used for player settlements
UCLASS()
class TOKYOCOA_API ACampBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ACampBase();

	// Stores a reference to the player character.
	void SetPlayerCharacter(TWeakObjectPtr<AAdventureCharacterBase> InPlayerCharacter);

	// Validates if the passed AActor is overlapping this settlement.
	bool ValidateActorOverlap(const AActor* Other);

	// Gets the radius of the settlement bounds sphere.
	float GetCampBoundsRadius() const;

protected:

	// Static mesh to hold the recruitment beacon mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SettlementBuilding")
	UStaticMeshComponent* RecruitmentBeacon = nullptr;

	// The sphere component marking the bounds of this settlement.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SettlementBuilding")
	USphereComponent* CampBounds = nullptr;

	virtual void BeginPlay() override;	

	// Overrides the OverlapBegin event.  Generally used to notify the player character that they have entered the settlement.
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Overrides the OverlapEnd event. Generally used to notify the player character that they have left the settlement.
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	
	// A reference to the player character.
	TWeakObjectPtr<AAdventureCharacterBase> PlayerCharacter = nullptr;

	// Gets a reference to the player character
	TWeakObjectPtr<AAdventureCharacterBase> GetPlayerCharacter();

	// Notifies the player character of the camp being created
	void NotifyPlayerBeginPlay();
};
