// Copyright 2024 DME Games


#include "CraftWidget.h"

#include "CraftItemSlotWidget.h"
#include "Components/VerticalBox.h"

void UCraftWidget::ClearVerticalBox() const
{
	CraftVerticalBox->ClearChildren();
}

void UCraftWidget::AddVerticalBoxChild(UCraftItemSlotWidget* WidgetToAdd) const
{
	if (WidgetToAdd)
	{
		CraftVerticalBox->AddChildToVerticalBox(WidgetToAdd);
	}
}
