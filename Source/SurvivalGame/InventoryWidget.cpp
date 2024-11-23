// Copyright 2024 DME Games

#include "InventoryWidget.h"

#include "InventoryComponent.h"
#include "InventoryItemGrid.h"
#include "InventoryItemSlot.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponentRef && WidgetItemSlot)
	{
		TArray<UInventoryItemSlot*> InventorySlotArray;
		
		InventoryToDisplay = InventoryComponentRef->GetInventory();
		for (int32 i = 0; i < InventoryToDisplay.Num(); ++i)
		{
			UInventoryItemSlot* NewItemSlot = CreateWidget<UInventoryItemSlot>(GetOwningPlayer(), WidgetItemSlot);
			NewItemSlot->SetNewSlotInfo(&InventoryToDisplay[i]);
			InventorySlotArray.Add(NewItemSlot);

			InventoryItemGrid->InventoryGrid->AddChildToUniformGrid(NewItemSlot, RowAsInt, ColumnAsInt);

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

void UInventoryWidget::SetInventoryRef(UInventoryComponent* InventoryIn)
{
	if (InventoryIn)
	{
		InventoryComponentRef = InventoryIn;
	}
}
