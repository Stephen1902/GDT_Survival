// Copyright 2024 DME Games


#include "ItemBaseActor.h"

#include "Components/SphereComponent.h"

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

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTableObj(TEXT("/Script/Engine.DataTable'/Game/Inventory/DT_Inventory.DT_Inventory'"));
	if (ItemTableObj.Succeeded())
	{
		ItemTableRow.DataTable = ItemTableObj.Object;


	}
}
#endif
// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBaseActor::OnInteract_Implementation(FInventoryStruct& ItemToInteractWith, bool& bCanPickUp)
{
	IBFI_Interactive::OnInteract_Implementation(ItemToInteractWith, bCanPickUp);

	UE_LOG(LogTemp, Warning, TEXT("Function successully called"));
	ItemToInteractWith = InventoryItem;
	bCanPickUp = true;
}

#if WITH_EDITOR
void AItemBaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (!bIsRandomItem)
	{
		if (ItemTableRow.RowName != FName("") && InventoryItem.ItemName == "")
		{
			if (FInventoryStruct* FoundRow = ItemTableRow.DataTable->FindRow<FInventoryStruct>(ItemTableRow.RowName, ""))
			{
				InventoryItem.ItemName = FoundRow->ItemName;
				InventoryItem.DisplayMesh = FoundRow->DisplayMesh;
				InventoryItem.Amount = FoundRow->Amount;
				InventoryItem.Icon = FoundRow->Icon;
				InventoryItem.bCanCraft = FoundRow->bCanCraft;

				MeshComponent->SetStaticMesh(FoundRow->DisplayMesh);
			}
		}
	}
	else
	{
		const TArray<FName> RandomArray = {"Wood", "Stone"};
		const int32 RandomArrayValue = FMath::RandRange(0, RandomArray.Num() - 1);
		const FName PickedArrayItem = RandomArray[RandomArrayValue]; 
		if (FInventoryStruct* FoundRow = ItemTableRow.DataTable->FindRow<FInventoryStruct>(PickedArrayItem, ""))
		{
			InventoryItem.ItemName = FoundRow->ItemName;
			InventoryItem.DisplayMesh = FoundRow->DisplayMesh;
			InventoryItem.Amount = FoundRow->Amount;
			InventoryItem.Icon = FoundRow->Icon;
			InventoryItem.bCanCraft = FoundRow->bCanCraft;

			MeshComponent->SetStaticMesh(FoundRow->DisplayMesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Row now found"));
		}
	}
}
#endif	

