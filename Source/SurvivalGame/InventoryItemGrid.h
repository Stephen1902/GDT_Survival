// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemGrid.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInventoryItemGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UBorder* InventoryBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UUniformGridPanel* InventoryGrid;
};
