// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UCraftWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Craft Widget", meta=(BindWidget))
	class UBorder* CraftBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Craft Widget", meta=(BindWidget))
	class UVerticalBox* CraftVerticalBox;

public:
	void ClearVerticalBox() const;
	void AddVerticalBoxChild(class UCraftItemSlotWidget* WidgetToAdd) const;
};
