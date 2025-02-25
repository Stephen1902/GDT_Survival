// Copyright 2024 DME Games


#include "InventoryComponent.h"

#include "BuildingComponent.h"
#include "InventoryWidget.h"
#include "SurvivalGameCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ItemDataTable = nullptr;
	InventoryWidgetRef = nullptr;
	TempMesh = nullptr;
	CurrentEquippedItem = nullptr;
	PlayerCharacterRef = nullptr;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(TempTimer, this, &UInventoryComponent::TempAddItems, 0.5f, false);

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(FInventoryStruct* ItemToAdd)
{
	bool bMatchFound = false;

	for (auto& InventoryLine : InventoryInfo)
	{
		if (InventoryLine.ItemName == ItemToAdd->ItemName)
		{
			InventoryLine.Amount = InventoryLine.Amount + ItemToAdd->Amount;
			bMatchFound = true;
			break;
		}
	}

	if (PlayerCharacterRef)
	{
		PlayerCharacterRef->DealWithNewItem(ItemToAdd->ItemName, ItemToAdd->Icon, ItemToAdd->Amount);
	}
	else
	{
		PlayerCharacterRef = Cast<ASurvivalGameCharacter>(GetOwner());
		PlayerCharacterRef->DealWithNewItem(ItemToAdd->ItemName, ItemToAdd->Icon, ItemToAdd->Amount);
	}

	if (!bMatchFound)
	{
		InventoryInfo.Add(*ItemToAdd);
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

bool UInventoryComponent::CheckItemExists(const FInventoryStruct* ItemToCheck)
{
	if (ItemToCheck)
	{
		for (int32 i = 0; i < InventoryInfo.Num(); ++i)
		{
			if (ItemToCheck->ItemName == InventoryInfo[i].ItemName)
			{
				return true;
			}
		}
	}
	
	return false;
}

void UInventoryComponent::DealWithInventoryButtonPress(APlayerController* PlayerControllerIn)
{
	if (InventoryWidget && PlayerControllerIn)
	{
		if (InventoryWidgetRef == nullptr)
		{
			InventoryWidgetRef = CreateWidget<UInventoryWidget>(PlayerControllerIn, InventoryWidget);
			InventoryWidgetRef->SetInventoryRef(this);
			InventoryWidgetRef->AddToViewport();
			PlayerControllerIn->SetShowMouseCursor(true);
			PlayerControllerIn->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			InventoryWidgetRef->RemoveFromParent();
			InventoryWidgetRef = nullptr;
			PlayerControllerIn->SetShowMouseCursor(false);
			PlayerControllerIn->SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to open the player widget but the widget or player controller is not valid"));
	}
}

void UInventoryComponent::SetEquippedItem(FInventoryStruct* SlotItem)
{
	CurrentEquippedItem = SlotItem;

	if (PlayerCharacterRef && CurrentEquippedItem->bCanBePlaced)
	{
		PlayerCharacterRef->GetBuildingComp()->EnterBuildMode(CurrentEquippedItem);
	}
}

void UInventoryComponent::SetPlayerCharacterRef(ASurvivalGameCharacter* ReferenceIn)
{
	PlayerCharacterRef = ReferenceIn;
}

ASurvivalGameCharacter* UInventoryComponent::GetPlayerCharacterRef() const
{
	if (PlayerCharacterRef)
	{
		return PlayerCharacterRef;
	}

	return  nullptr;
}

bool UInventoryComponent::CanCraft(FInventoryStruct* InventoryStructIn)
{
	bool bReturnCraft = true;

	// Check if there are any items in the data table used for crafting
	if (InventoryStructIn->CraftingItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to craft an item but there are no CraftingItems in the array."))
		bReturnCraft = false;
	}
	else
	{
		// Check if there are any items currently in the array
		if (InventoryInfo.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has nothing in their inventory."));
			bReturnCraft = false;
		}
		else
		{
			for (int32 i = 0; i < InventoryStructIn->CraftingItems.Num(); ++i)
			{
				// Check if there is an item matched on the name
				bool bMatchedItem = false;
				
				for (int32 j = 0; j < InventoryInfo.Num(); ++j)
				{
					UE_LOG(LogTemp, Warning, TEXT("Crafting Item is %s, Inventory Item is %s"), *InventoryStructIn->CraftingItems[i].InventoryItem.RowName.ToString(), *InventoryInfo[j].ItemName);
					// Check if the item in the inventory matches the items in the crafting requirements
					if (InventoryStructIn->CraftingItems[i].InventoryItem.RowName.ToString() == InventoryInfo[j].ItemName)
					{
						bMatchedItem = true;
						// Check if the amount in the inventory is enough to craft the item
						if (InventoryStructIn->CraftingItems[i].NumberRequired > InventoryInfo[j].Amount)
						{
							bReturnCraft = false;
							break;
						}
					}
				}
				
				if (!bMatchedItem)
				{
					bReturnCraft = false;
					break;
				}
			}
		}
	}
	
	return  bReturnCraft;
}

void UInventoryComponent::TempAddItems()
{
	GetWorld()->GetTimerManager().ClearTimer(TempTimer);

	if (FInventoryStruct* NewRow = ItemDataTable->FindRow<FInventoryStruct>("Wood", ""))
	{
		FInventoryStruct NewStruct;
		NewStruct.ItemName = NewRow->ItemName;
		NewStruct.Amount = 16;
		NewStruct.DisplayMesh = NewRow->DisplayMesh;
		NewStruct.Icon = NewRow->Icon;
		NewStruct.bCanCraft = NewRow->bCanCraft;
		NewStruct.CraftingItems = NewRow->CraftingItems;
		AddItem(&NewStruct);

		if ((NewRow = ItemDataTable->FindRow<FInventoryStruct>("Stone", "")))
		{
			NewStruct.ItemName = NewRow->ItemName;
			NewStruct.Amount = NewRow->Amount;
			NewStruct.DisplayMesh = NewRow->DisplayMesh;
			NewStruct.Icon = NewRow->Icon;
			NewStruct.bCanCraft = NewRow->bCanCraft;
			NewStruct.CraftingItems = NewRow->CraftingItems;
			AddItem(&NewStruct);
		}
	}

}

