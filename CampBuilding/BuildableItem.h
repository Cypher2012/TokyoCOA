#pragma once

// Engine classes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Custom classes


#include "BuildableItem.generated.h"

// Parent tree - this/SettlementBuilderManager (Its the builder manager that will spawn these)

// The base class of the buildable items
UCLASS()
class TOKYOCOA_API ABuildableItem : public AActor
{
	GENERATED_BODY()
	
public:	

	ABuildableItem();

	// Gets a reference to the static mesh component.
	UStaticMeshComponent* GetStaticDisplayMesh() const;

protected:

	// Static mesh that will be used for any static representation.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SettlementBuilding")
	UStaticMeshComponent* StaticDisplayMesh;

public:	

};
