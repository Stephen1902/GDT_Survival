// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UStatsWidget;
class UCanvasPanel;
class UImage;
class UOverlay;
class UHorizontalBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UStatsWidget* StatsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* DamageImage;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UOverlay* EquippedItemOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* EquippedItemBackgroundImage;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UHorizontalBox* EquippedItemHorizontalBox;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* EquippedItemIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UTextBlock* EquippedItemText;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UCanvasPanel* NewItemCanvasPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* NewItemBackground;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* NewItemIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UTextBlock* NewItemPlusText;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	UTextBlock* NewItemTextBlock;	
	
	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* NewItemAnimation;

	/** Sound to play when the player is being hurt */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	USoundBase* PlayerDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	USoundBase* NewItemSound;

public:
	UStatsWidget* GetStatsWidget() const { return StatsWidget; }

	void PlayDamageAnim();
	void PlayNewItemAnim();
	void SetNewItemInfo(const FString ItemName, UTexture2D* ItemIcon, const int32 ItemQuantity);
	
	void UpdateEquippedItem(const struct FInventoryStruct* ItemInfo);
};
