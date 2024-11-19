// Copyright 2024 DME Games


#include "InventoryComponent.h"

#include "InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ItemDataTable = nullptr;
	InventoryWidgetRef = nullptr;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FInventoryStruct NewStruct;
	NewStruct.ItemName = "Wood";
	NewStruct.Amount = 1;
	NewStruct.DisplayMesh = nullptr;
	NewStruct.Icon = nullptr;
	NewStruct.bCanCraft = false;
	AddItem(NewStruct);
	
	NewStruct.ItemName = "Stone";
	NewStruct.Amount = 3;
	AddItem(NewStruct);

	NewStruct.ItemName = "Leaves";
	NewStruct.Amount = 5;
	AddItem(NewStruct);

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(FInventoryStruct ItemToAdd)
{
	
	if (InventoryInfo.Num() > 0 && !ItemToAdd.ItemName.IsEmpty())
	{
		bool bMatchFound = false;

		for (auto& InventoryLine : InventoryInfo)
		{
			if (InventoryLine.ItemName == ItemToAdd.ItemName)
			{
				InventoryLine.Amount = InventoryLine.Amount + ItemToAdd.Amount;
				bMatchFound = true;
				break;
			}
		}
		
		if (!bMatchFound)
		{
			InventoryInfo.Add(ItemToAdd);
		}
	}
	else
	{
		InventoryInfo.Add(ItemToAdd);
	}

}

bool UInventoryComponent::RemoveItem(FString ItemToRemove, int32 AmountToRemove)
{
	bool bItemRemoved = false;

	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InventoryInfo[i].ItemName == ItemToRemove)
		{
			if (AmountToRemove > InventoryInfo[i].Amount)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attempting to remove more than is available.  Failed."));
			}
			else if (AmountToRemove == InventoryInfo[i].Amount)
			{
				TArray<FInventoryStruct> TempArray;
				for (int32 j = 0; j < InventoryInfo.Num(); ++j)
				{
					if (i != j)
					{
						TempArray.Add(InventoryInfo[j]);
					}
				}
				InventoryInfo.Empty();
				InventoryInfo = TempArray;
			
				bItemRemoved = true;
				break;
			}
			else
			{
				InventoryInfo[i].Amount = FMath::Clamp(InventoryInfo[i].Amount - AmountToRemove, 0, 1024);
				break;
			}
		}
	}
	
	return bItemRemoved;
}

void UInventoryComponent::DealWithInventoryButtonPress(APlayerController* PlayerControllerIn)
{
	if (InventoryWidget && PlayerControllerIn)
	{
		if (InventoryWidgetRef == nullptr)
		{
			InventoryWidgetRef = CreateWidget<UInventoryWidget>(PlayerControllerIn, InventoryWidget);
			InventoryWidgetRef->SetInventory(InventoryInfo);
			InventoryWidgetRef->AddToViewport();
		}
		else
		{
			InventoryWidgetRef->RemoveFromParent();
			InventoryWidgetRef = nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to open the player widget but the widget or player controller is not valid"));
	}
}

