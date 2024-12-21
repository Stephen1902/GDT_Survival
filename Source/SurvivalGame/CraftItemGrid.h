// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftItemGrid.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UCraftItemGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UBorder* CraftBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta=(BindWidget))
	class UScrollBox* CraftGrid;

public:
	UScrollBox* GetCraftGrid() const { return CraftGrid; }

};
