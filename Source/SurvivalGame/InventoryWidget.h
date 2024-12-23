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
	class UCraftItemGrid* CraftItemGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UCraftWidget* CraftWidgetBP;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UButton* CraftButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UBorder* CharacterBorder;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UImage* CharacterImage;

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
	FInventoryStruct* ItemToCraft;

	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnCraftButtonClicked();

	int32 RowAsInt = 0;
	int32 ColumnAsInt = 0;

	void DisplayInventory();
	void DisplayCraftedItems();
	//void DisplayCraftingResources(TArray<FCraftingItem*> ResourceStructArray, FInventoryStruct* ItemToCraft);
public:
	void SetInventoryRef(UInventoryComponent* InventoryIn);
	void UpdateCratedItemNeeds(FInventoryStruct* ItemToUpdate);
};
