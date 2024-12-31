// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BFL_Inventory.h"
#include "InventoryItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UButton* ItemButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UVerticalBox* ItemVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UImage* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UTextBlock* ItemText;

	// Color to change the button to when it is equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Widget")
	FLinearColor EquippedColour;
	
	FInventoryStruct* ItemToUse;	
	int32 QtyToDisplay;
private:
	FTimerHandle UpdateDelayHandle;

	virtual void NativeConstruct() override;
	
	void UpdateSlot();

	UFUNCTION()
	void ButtonPressed();

public:
	void SetNewSlotInfo(FInventoryStruct* ItemIn, int32 ItemAmount);

	void SetButtonStyle();

	FString GetSlotName() const { return ItemToUse->ItemName; }
};
