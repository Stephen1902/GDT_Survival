// Copyright 2024 DME Games


#include "FoliageMeshComponent.h"

#include "SurvivalGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void UFoliageMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacterRef = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to get a player reference."), *GetName());
	}

	for (int32 i = 0; i < GetInstanceCount(); ++i)
	{
		CurrentHealth.Add(StartingHealth);
	}
}

void UFoliageMeshComponent::OnInteractFoliage_Implementation(int32 FoliageInteractedWith, float DamageCaused, FInventoryStruct& ItemToInteractWith)
{
	IBFI_Interactive::OnInteractFoliage_Implementation(FoliageInteractedWith, DamageCaused, ItemToInteractWith);

	if (DamageCaused > 0.f && !DataTableRowHandle.IsNull())
	{
		if (FInventoryStruct* Row = DataTableRowHandle.DataTable->FindRow<FInventoryStruct>(DataTableRowHandle.RowName, ""))
		{
			ItemToInteractWith.ItemName = DataTableRowHandle.RowName.ToString();
			const int32 AmountToAdd = FMath::RandRange(2, 5);
			ItemToInteractWith.Amount = AmountToAdd;
			ItemToInteractWith.DisplayMesh = Row->DisplayMesh;
			ItemToInteractWith.Icon = Row->Icon;
			ItemToInteractWith.bCanCraft = Row->bCanCraft;
		}

		if (TreeHittingSound)
		{
			FTransform InstanceTransform;
			GetInstanceTransform(FoliageInteractedWith, InstanceTransform);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TreeHittingSound, InstanceTransform.GetLocation());
		}
		
		CurrentHealth[FoliageInteractedWith] -= DamageCaused;
		if (CurrentHealth[FoliageInteractedWith] <= 0.f)
		{
			RemoveInstance(FoliageInteractedWith);	
		}
	}
}
