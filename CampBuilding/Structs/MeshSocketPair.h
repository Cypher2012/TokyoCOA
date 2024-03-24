// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MeshSocketPair.generated.h"

class UStaticMeshSocket;

// A struct used to keep track of the MeshComponent that a socket is associated with.
USTRUCT()
struct FMeshSocketPair
{
	GENERATED_BODY()

	// A weak pointer to the owner of this mesh socket
	TWeakObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;

	// A weak pointer to the socket component
	TWeakObjectPtr<const UStaticMeshSocket> Socket = nullptr;

	FMeshSocketPair();

	FMeshSocketPair(TWeakObjectPtr<UStaticMeshComponent> InMeshComponent, TWeakObjectPtr<const UStaticMeshSocket> InMeshSocket);
};