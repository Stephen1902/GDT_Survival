// Copyright 2024 DME Games

#include "InventoryWidget.h"

#include "CraftItemGrid.h"
#include "CraftItemSlotWidget.h"
#include "CraftWidget.h"
#include "InventoryComponent.h"
#include "InventoryItemGrid.h"
#include "InventoryItemSlot.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DisplayInventory();
	DisplayCraftedItems();

}

void UInventoryWidget::DisplayInventory()
{
	ColumnAsInt = 0;
	RowAsInt = 0;
	
	if (InventoryComponentRef && WidgetItemSlot)
	{
		TArray<UInventoryItemSlot*> InventorySlotArray;
		
		InventoryToDisplay = InventoryComponentRef->GetInventory();
		for (int32 i = 0; i < InventoryToDisplay.Num(); ++i)
		{
			UInventoryItemSlot* NewItemSlot = CreateWidget<UInventoryItemSlot>(GetOwningPlayer(), WidgetItemSlot);
			NewItemSlot->SetNewSlotInfo(&InventoryToDisplay[i]);
			InventorySlotArray.Add(NewItemSlot);

			InventoryItemGrid->GetInventoryGrid()->AddChildToUniformGrid(NewItemSlot, RowAsInt, ColumnAsInt);

			ColumnAsInt += 1;
			if (ColumnAsInt > GridSlotsPerRow)
			{
				ColumnAsInt = 0;
				RowAsInt += 1;
			}
		}

		CurrentSetItem = InventoryComponentRef->GetCurrentEquippedItem();
		if (CurrentSetItem != nullptr && InventorySlotArray.Num() > 0)
		{
			for (int32 i = 0; i < InventorySlotArray.Num(); ++i)
			{
				if (CurrentSetItem->ItemName == InventorySlotArray[i]->GetSlotName())
				{
					InventorySlotArray[i]->SetButtonStyle();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called on InventoryWidget but InventoryToDisplay has 0 entries or WidgetItemSlot has not been set."));
	}
}

void UInventoryWidget::DisplayCraftedItems()
{
	if (InventoryComponentRef && CraftItemSlot)
	{
		ColumnAsInt = 0;
		RowAsInt = 0;

		CraftItemGrid->GetCraftGrid()->ClearChildren();
		if (const UDataTable* CraftDataTable = InventoryComponentRef->GetDataTable())
		{
			TArray<FName> TableRows = CraftDataTable->GetRowNames();
			if (TableRows.Num() > 0)
			{
				for (const auto& It : TableRows)
				{
					if (FInventoryStruct* Row = CraftDataTable->FindRow<FInventoryStruct>(It, ""))
					{
						if (Row->bCanCraft)
						{
							UCraftItemSlotWidget* NewSlot = CreateWidget<UCraftItemSlotWidget>(GetOwningPlayer(), CraftItemSlot);
							NewSlot->SetSlotInfo(Row);
							NewSlot->SetInventoryWidgetRef(this);
							CraftItemGrid->GetCraftGrid()->AddChild(NewSlot);
						}
					}
						
				}
			}
		}
	}
}

void UInventoryWidget::DisplayCraftingResources(TArray<FCraftingItem*> ResourceStructArray, FInventoryStruct* ItemToCraft)
{
	CraftWidgetBP->ClearVerticalBox();

	if (CraftItemSlot)
	{
		for (auto It : ResourceStructArray)
		{
			if (FInventoryStruct* Row = It->InventoryItem.DataTable->FindRow<FInventoryStruct>(It->InventoryItem.RowName, ""))
			{
				UCraftItemSlotWidget* NewSlot = CreateWidget<UCraftItemSlotWidget>(GetOwningPlayer(), CraftItemSlot);
				CraftWidgetBP->AddVerticalBoxChild(NewSlot);
				NewSlot->SetSlotInfo(Row);
			}
		}
	}
}

void UInventoryWidget::SetInventoryRef(UInventoryComponent* InventoryIn)
{
	if (InventoryIn)
	{
		InventoryComponentRef = InventoryIn;
	}
}

void UInventoryWidget::UpdateCratedItemNeeds(FInventoryStruct* ItemToUpdate)
{
	
}
