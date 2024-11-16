// Copyright 2024 DME Games


#include "InventoryItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UInventoryItemSlot::UpdateAfterDelay, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}
}

void UInventoryItemSlot::UpdateAfterDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		ItemImage->SetBrushFromSoftTexture(ItemToUse->Icon);
		ItemText->SetText(FText::FromString(FString::FromInt(ItemToUse->Amount)));
	}
}
