// Copyright 2024 DME Games

#include "PlayerWidget.h"

#include "BFL_Inventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerWidget::PlayDamageAnim()
{
	if (DamageAnimation && !IsAnimationPlaying(DamageAnimation))
	{
		PlayAnimation(DamageAnimation);

		if (PlayerDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerDamageSound, GetOwningPlayer()->GetPawn()->GetActorLocation());
		}
	}
}

void UPlayerWidget::PlayNewItemAnim()
{
	if (!IsAnimationPlaying(NewItemAnimation))
	{
		PlayAnimation(NewItemAnimation);

		if (NewItemSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerDamageSound, GetOwningPlayer()->GetPawn()->GetActorLocation());
		}
	}
}

void UPlayerWidget::SetNewItemInfo(const FString ItemName, UTexture2D* ItemIcon, const int32 ItemQuantity)
{
	const FString StringToAdd = ItemName + " (" + FString::FromInt(ItemQuantity) + ")";
	NewItemTextBlock->SetText(FText::FromString(StringToAdd));
	NewItemIcon->SetBrushFromTexture(ItemIcon);
}

void UPlayerWidget::UpdateEquippedItem(const FInventoryStruct* ItemInfo)
{
	if (ItemInfo != nullptr)
	{
		EquippedItemIcon->SetBrushFromTexture(ItemInfo->Icon);
		EquippedItemText->SetText(FText::FromString(ItemInfo->ItemName));
	}
}
