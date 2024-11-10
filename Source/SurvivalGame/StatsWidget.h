// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UStatsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player Stats", meta=(BindWidget))
	UProgressBar* PB_Health;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats", meta=(BindWidget))
	UProgressBar* PB_Stamina;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats", meta=(BindWidget))
	UProgressBar* PB_Food;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats", meta=(BindWidget))
	UProgressBar* PB_Water;

public:
	UProgressBar* GetHealthBar() const { return PB_Health; }
	UProgressBar* GetStaminaBar() const { return PB_Stamina; }
	UProgressBar* GetFoodBar() const { return PB_Food; }
	UProgressBar* GetWaterBar() const { return PB_Water; }
};
