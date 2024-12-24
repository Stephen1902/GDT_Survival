// Copyright 2024 DME Games


#include "BuildingBaseClass.h"

// Sets default values
ABuildingBaseClass::ABuildingBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshToDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshToDisplay->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ABuildingBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingBaseClass::SetDisplayMesh(UStaticMesh* MeshIn) const
{
	if (MeshIn)
	{
		MeshToDisplay->SetStaticMesh(MeshIn);
	}
}
