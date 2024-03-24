// Fill out your copyright notice in the Description page of Project Settings.


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