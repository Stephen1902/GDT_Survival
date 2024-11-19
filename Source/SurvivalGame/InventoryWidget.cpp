// Copyright 2024 DME Games


#include "InventoryWidget.h"
#include "InventoryItemGrid.h"
#include "InventoryItemSlot.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/KismetMathLibrary.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryToDisplay.Num() > 0 && WidgetItemSlot)
	{
		for (int32 i = 0; i < InventoryToDisplay.Num(); ++i)
		{
			UInventoryItemSlot* NewItemSlot = CreateWidget<UInventoryItemSlot>(GetOwningPlayer(), WidgetItemSlot);
			NewItemSlot->SetNewSlotInfo(&InventoryToDisplay[i]);

			const double IAsDouble = UKismetMathLibrary::Conv_IntToDouble(i);
			const double GridSlotsAsDouble = UKismetMathLibrary::Conv_IntToDouble(GridSlotsPerRow); 
			 
			const int32 RowAsInt = UKismetMathLibrary::FTrunc(IAsDouble / GridSlotsAsDouble);
			double ColumnRemainder;
			UKismetMathLibrary::FMod(GridSlotsAsDouble, IAsDouble, ColumnRemainder);
			const int32 ColumnAsInt = UKismetMathLibrary::FTrunc(ColumnRemainder);
			
			InventoryItemGrid->InventoryGrid->AddChildToUniformGrid(NewItemSlot, RowAsInt, ColumnAsInt);
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called on InventoryWidget but InventoryToDisplay has 0 entries or WidgetItemSlot has not been set."));
	}

	UE_LOG(LogTemp, Warning, TEXT("Panel has %i children."), InventoryItemGrid->InventoryGrid->GetChildrenCount());
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
