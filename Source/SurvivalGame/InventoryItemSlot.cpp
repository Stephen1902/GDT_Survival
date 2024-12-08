// Copyright 2024 DME Games

#include "InventoryItemSlot.h"
#include "SurvivalGameCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateColor.h"

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDelayHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateDelayHandle, this, &UInventoryItemSlot::UpdateSlot, GetWorld()->GetDeltaSeconds(), false, GetWorld()->GetDeltaSeconds());
	}

	ItemButton->OnClicked.AddDynamic(this, &UInventoryItemSlot::ButtonPressed);
}

void UInventoryItemSlot::UpdateSlot()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelayHandle);

	if (ItemToUse)
	{
		ItemImage->SetBrushFromTexture(ItemToUse->Icon);
		ItemText->SetText(FText::FromString(FString::FromInt(ItemToUse->Amount)));
	}
}

void UInventoryItemSlot::ButtonPressed()
{
	if (ItemToUse)
	{
		if (ASurvivalGameCharacter* PlayerChar = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			PlayerChar->SetEquippedItemMesh(ItemToUse);
		}

		SetButtonStyle();
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

void UInventoryItemSlot::SetButtonStyle()
{
	FButtonStyle ButtonStyle = ItemButton->GetStyle();
	FSlateBrush ButtonBrush = ButtonStyle.Normal;
	ButtonBrush.TintColor = FSlateColor(FLinearColor(EquippedColour));
	ButtonStyle.Normal = ButtonBrush;
	ItemButton->SetStyle(ButtonStyle);
	
}
