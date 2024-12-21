// Copyright 2024 DME Games


#include "CraftItemSlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "SurvivalGameCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCraftItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UCraftItemSlotWidget::UpdateSlot, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}

	CraftButton->OnClicked.AddDynamic(this, &UCraftItemSlotWidget::ButtonPressed);
}

void UCraftItemSlotWidget::UpdateSlot()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		CraftImage->SetBrushFromTexture(ItemToUse->Icon);
		CraftNameText->SetText(FText::FromString(ItemToUse->ItemName));

		if (!PlayerRef)
		{
			PlayerRef = Cast<ASurvivalGameCharacter>(GetOwningPlayer()->GetCharacter());
		}
		
		if (PlayerRef)
		{
			SetButtonStyle(PlayerRef->GetInventoryComp()->CanCraft(ItemToUse));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CraftItemSlotWidget does not have a PlayerRef"));
		}
	}
	
}

void UCraftItemSlotWidget::ButtonPressed()
{
	if (InventoryWidgetRef)
	{
		InventoryWidgetRef->UpdateCratedItemNeeds(ItemToUse);
	}
}

void UCraftItemSlotWidget::SetSlotInfo(FInventoryStruct* ItemInfo)
{
	if (ItemInfo != nullptr)
	{
		ItemToUse = ItemInfo;
		UpdateSlot();
	}
}

void UCraftItemSlotWidget::SetInventoryWidgetRef(UInventoryWidget* ReferenceIn)
{
	if (ReferenceIn)
	{
		InventoryWidgetRef = ReferenceIn;
	}
}

void UCraftItemSlotWidget::SetButtonStyle(const bool CanCraft)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemToUse is %s and CanCraft is %s"), *ItemToUse->ItemName, CanCraft ? TEXT("true") : TEXT("false"));
	FButtonStyle ButtonStyle = CraftButton->GetStyle();
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
	CraftButton->SetStyle(ButtonStyle);
}
