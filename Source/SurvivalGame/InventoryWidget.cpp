// Copyright 2024 DME Games

#include "InventoryWidget.h"
#include "InventoryItemGrid.h"
#include "InventoryItemSlot.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryToDisplay.Num() > 0 && WidgetItemSlot)
	{
		for (int32 i = 0; i < InventoryToDisplay.Num(); ++i)
		{
			UInventoryItemSlot* NewItemSlot = CreateWidget<UInventoryItemSlot>(GetOwningPlayer(), WidgetItemSlot);
			NewItemSlot->SetNewSlotInfo(&InventoryToDisplay[i]);

			InventoryItemGrid->InventoryGrid->AddChildToUniformGrid(NewItemSlot, RowAsInt, ColumnAsInt);

			ColumnAsInt += 1;
			if (ColumnAsInt > GridSlotsPerRow)
			{
				ColumnAsInt = 0;
				RowAsInt += 1;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called on InventoryWidget but InventoryToDisplay has 0 entries or WidgetItemSlot has not been set."));
	}
}

void UInventoryWidget::SetInventory(const TArray<FInventoryStruct>& InventoryIn)
{
	if (InventoryIn.IsValidIndex(0))
	{
		InventoryToDisplay = InventoryIn;
		UE_LOG(LogTemp, Warning, TEXT("InventoryToDisplay set with %i entries."), InventoryToDisplay.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryIn contains no entries."))
	}

	
}
