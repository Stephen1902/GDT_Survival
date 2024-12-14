// Copyright 2024 DME Games


#include "CraftItemSlotWidget.h"
#include "InventoryComponent.h"
#include "SurvivalGameCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCraftItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UCraftItemSlotWidget::UpdateSlot, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}

	ItemButton->OnClicked.AddDynamic(this, &UCraftItemSlotWidget::ButtonPressed);
}

void UCraftItemSlotWidget::UpdateSlot()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		ItemImage->SetBrushFromTexture(ItemToUse->Icon);

		if (PlayerRef)
		{
			SetButtonStyle(PlayerRef->GetInventoryComp()->CanCraft(ItemToUse));
		}
	}
}

void UCraftItemSlotWidget::ButtonPressed()
{
}

void UCraftItemSlotWidget::SetSlotInfo(ASurvivalGameCharacter* PlayerRefIn, FInventoryStruct* ItemInfo)
{
	if (PlayerRefIn)
	{
		PlayerRef = PlayerRefIn;
	}

	if (ItemInfo != nullptr)
	{
		ItemToUse = ItemInfo;
		UpdateSlot();
	}
}

void UCraftItemSlotWidget::SetButtonStyle(const bool CanCraft)
{
	FButtonStyle ButtonStyle = ItemButton->GetStyle();
	FSlateBrush ButtonBrush = ButtonStyle.Normal;
	if (CanCraft)
	{
		ButtonBrush.TintColor = FSlateColor(FLinearColor(CanCraftColour));
	}
	else
	{
		ButtonBrush.TintColor = FSlateColor(FLinearColor(CannotCraftColour));
	}
	ButtonStyle.Normal = ButtonBrush;
	ItemButton->SetStyle(ButtonStyle);
}
