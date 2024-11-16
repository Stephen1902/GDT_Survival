// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UInventoryItemGrid* InventoryItemGrid;
};
