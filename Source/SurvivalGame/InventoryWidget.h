// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BFL_Inventory.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UInventoryItemGrid* InventoryItemGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	UInventoryItemGrid* CraftItemGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UCraftWidget* CraftWidgetBP;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UButton* CraftButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget")
	TSubclassOf<class UInventoryItemSlot> WidgetItemSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget")
	TSubclassOf<class UCraftItemSlotWidget> CraftItemSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget")
	int32 GridSlotsPerRow = 5;
private:
	TArray<FInventoryStruct> InventoryToDisplay;
	
	UPROPERTY()
	UInventoryComponent* InventoryComponentRef;

	FInventoryStruct* CurrentSetItem;

	virtual void NativeConstruct() override;

	int32 RowAsInt = 0;
	int32 ColumnAsInt = 0;
public:
	void SetInventoryRef(UInventoryComponent* InventoryIn);
};
