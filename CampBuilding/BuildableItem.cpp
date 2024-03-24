// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.

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
