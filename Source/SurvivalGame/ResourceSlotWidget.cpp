// Copyright 2024 DME Games


#include "ResourceSlotWidget.h"

#include "BFL_Inventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UResourceSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UResourceSlotWidget::UpdateSlot, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}

	ItemButton->OnClicked.AddDynamic(this, &UResourceSlotWidget::ButtonPressed);
}

void UResourceSlotWidget::UpdateSlot()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		ItemImage->SetBrushFromTexture(ItemToUse->Icon);
		const FString TextToDisplay = ItemToUse->ItemName + " : " + FString::FromInt(ItemToUse->Amount);
		ItemText->SetText(FText::FromString(*TextToDisplay));
	}
}

void UResourceSlotWidget::ButtonPressed()
{
	if (ItemToUse)
	{

	}
}

void UResourceSlotWidget::SetNewSlotInfo(FInventoryStruct* ItemInfo)
{
	if (ItemInfo != nullptr)
	{
		ItemToUse = ItemInfo;
		UpdateSlot();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to call SetNewSlotInfo but ItemInfo was not valid."));
	}
}
