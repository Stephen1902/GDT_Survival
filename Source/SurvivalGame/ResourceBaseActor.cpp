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

void AResourceBaseActor::OnInteract_Implementation(float Damage, FInventoryStruct& ItemToInteractWith, APlayerController* Controller, AActor* DamageCauser)
{
	IBFI_Interactive::OnInteract_Implementation(Damage, ItemToInteractWith, Controller, DamageCauser);

	if (ItemsToGive.Num() > 0)
	{
		int32 RandomPickup = 0;

		if (ItemsToGive.Num() > 1)
		{
			RandomPickup = FMath::RandRange(0, ItemsToGive.Num() - 1);
		}

		if (FInventoryStruct* Row = ItemsToGive[RandomPickup].DataTable->FindRow<FInventoryStruct>(ItemsToGive[RandomPickup].RowName, ""))
		{
			UE_LOG(LogTemp, Warning, TEXT("Row Found"));
			ItemToInteractWith.ItemName = Row->ItemName;
			ItemToInteractWith.Amount = 1;
			ItemToInteractWith.DisplayMesh = Row->DisplayMesh;
			ItemToInteractWith.Icon = Row->Icon;
			ItemToInteractWith.bCanCraft = Row->bCanCraft;
			ItemToInteractWith.CraftingItems = Row->CraftingItems;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get a valid row in ItemBaseActor OnInteract."));
		}

		if (Damage > 0.f)
		{
			ResourceHealth -= Damage;
			UE_LOG(LogTemp, Warning, TEXT("Health is now %f."), ResourceHealth);
		}

		if (ResourceHealth <= 0.f)
		{
			Destroy();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact called on %s but there are no items in the inventory."), *GetName());
	}
}

