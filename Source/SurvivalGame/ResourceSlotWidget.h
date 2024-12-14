// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BFL_Inventory.h"
#include "ResourceSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UResourceSlotWidget : public UUserWidget
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
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Widget", meta=(ExposeOnSpawn))
	FInventoryStruct* ItemToUse;

private:
	FTimerHandle UpdateDelayHandle;

	virtual void NativeConstruct() override;
	
	void UpdateSlot();

	UFUNCTION()
	void ButtonPressed();
public:
	void SetNewSlotInfo(FInventoryStruct* ItemInfo);
};
