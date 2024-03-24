// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableItem.h"

ABuildableItem::ABuildableItem()
{
	StaticDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticDisplayMesh");

	RootComponent = StaticDisplayMesh;
}

UStaticMeshComponent* ABuildableItem::GetStaticDisplayMesh() const
{
	return StaticDisplayMesh;
}
