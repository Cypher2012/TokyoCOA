// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameHUDInterface.generated.h"

class ACoreGameHUD;

UINTERFACE(MinimalAPI, Blueprintable)
class UGameHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class IGameHUDInterface
{
	GENERATED_BODY()

public:

	// Return the core game HUD
	virtual ACoreGameHUD* GetCoreGameHUD();

};