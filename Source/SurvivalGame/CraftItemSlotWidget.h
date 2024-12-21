// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "BFL_Inventory.h"
#include "Blueprint/UserWidget.h"
#include "CraftItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UCraftItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget", meta=(BindWidget))
	class UButton* CraftButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget", meta=(BindWidget))
	class UVerticalBox* CraftVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget", meta=(BindWidget))
	class UImage* CraftImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget", meta=(BindWidget))
	class UTextBlock* CraftNameText;
	
	// Color to change the button to when it is equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget")
	FLinearColor CanCraftColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Widget")
	FLinearColor CannotCraftColour;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Widget", meta=(ExposeOnSpawn))
	FInventoryStruct* ItemToUse;
private:
	FTimerHandle UpdateDelayHandle;

	virtual void NativeConstruct() override;
	
	void UpdateSlot();

	UFUNCTION()
	void ButtonPressed();

	UPROPERTY()
	class ASurvivalGameCharacter* PlayerRef;

	UPROPERTY()
	class UInventoryWidget* InventoryWidgetRef;

public:
	void SetSlotInfo(FInventoryStruct* ItemInfo);
	void SetInventoryWidgetRef(UInventoryWidget* ReferenceIn);

	void SetButtonStyle(const bool CanCraft);

	FString GetSlotName() const { return ItemToUse->ItemName; }
};

