// Copyright 2024 DME Games


#include "ItemBaseActor.h"
#include "GameFramework/Controller.h"
#include "SurvivalGameCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
#if WITH_EDITOR
AItemBaseActor::AItemBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	OverlapComp = CreateDefaultSubobject<USphereComponent>("Collsion Comp");
	OverlapComp->SetupAttachment(RootComp);
	OverlapComp->SetCollisionProfileName(FName("OverlapAllDynamic"), true);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComponent->SetupAttachment(RootComp);

	bItemIsPickedUp = false;
	StartingHealth = 5.0f;
	CurrentHealth = StartingHealth;
}
#endif
// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	/*OnTakeAnyDamage.AddDynamic(this, &AItemBaseActor::OnDamageTaken);*/
}

// Called every frame
void AItemBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBaseActor::OnInteract_Implementation(float Damage, FInventoryStruct& ItemToInteractWith, APlayerController* Controller, AActor* DamageCauser)
{
	// Check that the inventory array has something in it
	if (InventoryItem.Num() > 0)
	{
		// We only want to do something here if the player action is correct ie interact with a stone that can be picked up or damage an object like a tree
		if ((bItemIsPickedUp && Damage == 0.f) || (!bItemIsPickedUp && Damage > 0.f))
		{
			IBFI_Interactive::OnInteract_Implementation(Damage, ItemToInteractWith, Controller, DamageCauser);

			int32 RandomPickup = 0;

			if (!bItemIsPickedUp && InventoryItem.Num() > 1)
			{
				RandomPickup = FMath::RandRange(0, InventoryItem.Num() - 1);
			}

			if (FInventoryStruct* Row = InventoryItem[RandomPickup].DataTable->FindRow<FInventoryStruct>(InventoryItem[RandomPickup].RowName, ""))
			{
				ItemToInteractWith.ItemName = Row->ItemName;
				ItemToInteractWith.Amount = 1;
				ItemToInteractWith.DisplayMesh = Row->DisplayMesh;
				ItemToInteractWith.Icon = Row->Icon;
				ItemToInteractWith.bCanCraft = Row->bCanCraft;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get a valid row in ItemBaseActor OnInteract."));
			}

			if (Damage > 0.f)
			{
				CurrentHealth -= Damage;
				UE_LOG(LogTemp, Warning, TEXT("Health is now %f."), CurrentHealth);
			}

			if (bItemIsPickedUp || CurrentHealth <= 0.f)
			{
				Destroy();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No information set in the array for %s."), *GetName());
	}
}

void AItemBaseActor::OnDamageTaken(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType,	AController* DamageInstigator, AActor* DamageCauser)
{
	if (DamageAmount > 0.f)
	{
		CurrentHealth -= DamageAmount;
	}
}

#if WITH_EDITOR
void AItemBaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bItemIsPickedUp)
	{
		StartingHealth = 0.f;
	}
	
	if (InventoryItem.Num() > 0)
	{
		if (FInventoryStruct* Row = InventoryItem[0].DataTable->FindRow<FInventoryStruct>(InventoryItem[0].RowName, ""))
		{
			MeshComponent->SetStaticMesh(Row->DisplayMesh);
		}
	}
}
#endif
