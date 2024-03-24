// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PDAPawn.generated.h"

class UCameraComponent;
class UPDAWidget;
class UWidgetComponent;

UCLASS()
class TOKYOCOA_API APDAPawn : public AActor
{
	GENERATED_BODY()
	
public:	

	APDAPawn();

	// Player wants to activate the PDA
	void EnablePDA();

	// Player wants to disable the PDA
	void DisablePDA();

protected:

	// Camera component for viewing the PDA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	UCameraComponent* PDACamera = nullptr;

	// Static mesh of the PDA body
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	UStaticMeshComponent* PDABodyMesh = nullptr;

	// Static mesh for the actual screen of the PDA. Used to render the UI to.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	UStaticMeshComponent* PDAScreenMesh = nullptr;

	// The 3D widget component that will be used to project the widget and render the texture.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PDA")
	UWidgetComponent* PDAWidget = nullptr;

	virtual void BeginPlay() override;

private:

	bool bIsEnabled = false;
};
