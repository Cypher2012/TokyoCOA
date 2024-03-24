// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

#include "MeshSocketPair.h"

#include "Engine/StaticMeshSocket.h"

FMeshSocketPair::FMeshSocketPair()
{
	//MeshComponent = nullptr;
	//Socket = nullptr;
}

FMeshSocketPair::FMeshSocketPair(TWeakObjectPtr<UStaticMeshComponent> InMeshComponent, TWeakObjectPtr<const UStaticMeshSocket> InMeshSocket)
{
	MeshComponent = InMeshComponent;
	Socket = InMeshSocket;
}