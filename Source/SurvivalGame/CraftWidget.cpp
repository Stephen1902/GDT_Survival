// Copyright 2024 DME Games


#include "CraftWidget.h"
#include "InventoryItemSlot.h"
#include "Components/VerticalBox.h"

void UCraftWidget::ClearVerticalBox() const
{
	CraftVerticalBox->ClearChildren();
}

void UCraftWidget::AddVerticalBoxChild(UInventoryItemSlot* WidgetToAdd) const
{
	if (WidgetToAdd)
	{
		CraftVerticalBox->AddChild(WidgetToAdd);
	}
}
