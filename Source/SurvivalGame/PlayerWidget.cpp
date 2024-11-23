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

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerDamageSound, GetOwningPlayer()->GetPawn()->GetActorLocation());
	}
}

void UPlayerWidget::UpdateEquippedItem(const FInventoryStruct* ItemInfo)
{
	if (ItemInfo != nullptr)
	{
		EquippedItemIcon->SetBrushFromTexture(ItemInfo->Icon);
		EquippedItemText->SetText(FText::FromString(ItemInfo->ItemName));
	}
}
