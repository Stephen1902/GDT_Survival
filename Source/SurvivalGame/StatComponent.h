// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

UCLASS( ClassGroup=(Stats), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StartingHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StartingHunger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StartingThirst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StartingStamina;

	// Stat recovery / drain amounts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float HealPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainPerSecond;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float FoodDropPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float WaterDropPerSecond;
	
	// Timer Frequencies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float HealTimerFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaTimerFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float FoodTimerFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float WaterTimerFrequency;

public:	
	UFUNCTION(BlueprintCallable, Category = "Stat Functions")
	void AdjustStat(float& StatToAdjust, float AmountToAdjust, float MaxValue, bool IsDamaging, float DamageAmount, class UProgressBar* WidgetToUpdate);

	UFUNCTION(BlueprintCallable, Category = "Stat Functions")
	void UpdateStatBar(float StatValue, float MaxValue, UProgressBar* WidgetToUpdate);

	void SetPlayerWidgetRef(class UPlayerWidget* ReferenceIn);
private:
	float Health;
	float Hunger;
	float Thirst;
	float Stamina;
	bool bIsHealing;
	bool bIsRegainingStamina;

	void CauseDamage(float DamageAmount);
	void Heal();
	void AdjustStamina();
	void FoodDrain();
	void WaterDrain();

	// Timer Handles
	FTimerHandle HealTimerHandle;
	FTimerHandle StaminaTimerHandle;
	FTimerHandle FoodTimerHandle;
	FTimerHandle WaterTimerHandle;

	UPROPERTY()
	class ASurvivalGameCharacter* PlayerCharacter;

	UPROPERTY()
	UPlayerWidget* PlayerWidgetRef;

public:
	void SetIsRegainingStamina(const bool NewStaminaState); 
};
