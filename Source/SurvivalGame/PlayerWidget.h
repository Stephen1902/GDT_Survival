// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"



/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	class UStatsWidget* StatsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", meta=(BindWidget))
	class UImage* DamageImage;

	UPROPERTY(EditDefaultsOnly, Category = "Player Widget", Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* DamageAnimation;

	/** Sound to play when the player is being hurt */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	USoundBase* PlayerDamageSound;

public:
	UStatsWidget* GetStatsWidget() const { return StatsWidget; }

	void PlayDamageAnim();
};
