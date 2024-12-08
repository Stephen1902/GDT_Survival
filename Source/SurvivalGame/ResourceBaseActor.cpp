// Copyright 2024 DME Games


#include "ResourceBaseActor.h"

// Sets default values
AResourceBaseActor::AResourceBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);

	ResourceHealth = 5.0f; 
}

// Called when the game starts or when spawned
void AResourceBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

