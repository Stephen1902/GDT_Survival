// Copyright 2024 DME Games


#include "InventoryItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UInventoryItemSlot::UpdateSlot, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}
}

void UInventoryItemSlot::UpdateSlot()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		ItemImage->SetBrushFromSoftTexture(ItemToUse->Icon);
		ItemText->SetText(FText::FromString(FString::FromInt(ItemToUse->Amount)));
	}
}

void UInventoryItemSlot::SetNewSlotInfo(FInventoryStruct* ItemInfo)
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
