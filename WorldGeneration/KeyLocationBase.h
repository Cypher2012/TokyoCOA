// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "KeyLocationBase.generated.h"

class USphereComponent;

// Parent tree - this/WorldGenerator/AdventurePlayGameMode

// The base class for unique key locations. During world generation, the world generator
// will spawn an instance of each of the classes. This base class has some features which allow the
// world generator and settlement spawner component know where they can place items.
UCLASS()
class TOKYOCOA_API AKeyLocationBase : public AActor
{
	GENERATED_BODY()
	
public:

	AKeyLocationBase();

	// Gets the radius of the ClearArea sphere.
	float GetLocationBoundsRadius() const;


protected:
	
	// The area around the location that needs to be kept clear
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "KeyLocation")
	USphereComponent* LocationBounds = nullptr;

	// Display name of the location
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KeyLocation")
	FName LocationName = "KeyLocationName";

	virtual void BeginPlay() override;

private:


};
