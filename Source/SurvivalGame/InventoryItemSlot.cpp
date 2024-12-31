// Copyright 2024 DME Games

#include "InventoryItemSlot.h"

#include "BuildingComponent.h"
#include "InventoryComponent.h"
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
		if (QtyToDisplay == -1)
		{
			ItemText->SetText(FText::FromString(FString::FromInt(ItemToUse->Amount)));
		}
		else
		{
			ItemText->SetText(FText::FromString(FString::FromInt(QtyToDisplay)));
		}
	}
}

void UInventoryItemSlot::ButtonPressed()
{
	if (ItemToUse)
	{
		if (ASurvivalGameCharacter* PlayerChar = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (ItemToUse->bCanBePlaced)
			{
				PlayerChar->GetBuildingComp()->EnterBuildMode(ItemToUse);
				PlayerChar->GetInventoryComp()->DealWithInventoryButtonPress(Cast<APlayerController>(PlayerChar->GetController()));
			}
			else
			{
				PlayerChar->SetEquippedItemMesh(ItemToUse);
			}

			SetButtonStyle();
		}
	}
}

void UInventoryItemSlot::SetNewSlotInfo(FInventoryStruct* ItemIn, int32 ItemAmount)
{
	if (ItemIn != nullptr)
	{
		ItemToUse = ItemIn;
		QtyToDisplay = ItemAmount;
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
