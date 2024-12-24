// Copyright 2024 DME Games


#include "BuildingBaseClass.h"

#include "Components/BoxComponent.h"

// Sets default values
ABuildingBaseClass::ABuildingBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshToDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshToDisplay->SetupAttachment(RootComp);

	FloorTraceSocket1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trace 1"));
	FloorTraceSocket1->SetupAttachment(RootComp);
	FloorTraceSocket1->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorTraceSocket1->SetBoxExtent(FVector(56.f));
	
	FloorTraceSocket2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trace 2"));
	FloorTraceSocket2->SetupAttachment(RootComp);
	FloorTraceSocket2->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorTraceSocket2->SetBoxExtent(FVector(56.f));
	
	FloorTraceSocket3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trace 3"));
	FloorTraceSocket3->SetupAttachment(RootComp);
	FloorTraceSocket3->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorTraceSocket3->SetBoxExtent(FVector(56.f));
	
	FloorTraceSocket4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trace 4"));
	FloorTraceSocket4->SetupAttachment(RootComp);
	FloorTraceSocket4->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorTraceSocket4->SetBoxExtent(FVector(56.f));

	WallTraceSocket1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Trace 1"));
	WallTraceSocket1->SetupAttachment(RootComp);
	WallTraceSocket1->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallTraceSocket1->SetBoxExtent(FVector(56.f));
	
	WallTraceSocket2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Trace 2"));
	WallTraceSocket2->SetupAttachment(RootComp);
	WallTraceSocket2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallTraceSocket2->SetBoxExtent(FVector(56.f));
	
	WallTraceSocket3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Trace 3"));
	WallTraceSocket3->SetupAttachment(RootComp);
	WallTraceSocket3->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallTraceSocket3->SetBoxExtent(FVector(56.f));
	
	WallTraceSocket4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Trace 4"));
	WallTraceSocket4->SetupAttachment(RootComp);
	WallTraceSocket4->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallTraceSocket4->SetBoxExtent(FVector(56.f));
	
}

// Called when the game starts or when spawned
void ABuildingBaseClass::BeginPlay()
{
	Super::BeginPlay();

	// Check whether the collision sockets have been moved and if they have, update their collision
	if (FloorTraceSocket1->GetRelativeLocation() != FVector::Zero())
	{
		FloorTraceSocket1->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	if (FloorTraceSocket2->GetRelativeLocation() != FVector::Zero())
	{
		FloorTraceSocket2->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	if (FloorTraceSocket3->GetRelativeLocation() != FVector::Zero())
	{
		FloorTraceSocket3->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	if (FloorTraceSocket4->GetRelativeLocation() != FVector::Zero())
	{
		FloorTraceSocket4->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	if (WallTraceSocket1->GetRelativeLocation() != FVector::Zero())
	{
		WallTraceSocket1->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	}

	if (WallTraceSocket2->GetRelativeLocation() != FVector::Zero())
	{
		WallTraceSocket2->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	}

	if (WallTraceSocket3->GetRelativeLocation() != FVector::Zero())
	{
		WallTraceSocket3->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	}

	if (WallTraceSocket4->GetRelativeLocation() != FVector::Zero())
	{
		WallTraceSocket4->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	}
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
