// Copyright 2024 DME Games


#include "StatComponent.h"
#include "PlayerWidget.h"
#include "StatsWidget.h"
#include "SurvivalGameCharacter.h"
#include "Components/ProgressBar.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	StartingHealth = 10.f;
	StartingHunger = 5.f;
	StartingStamina = 80.f;
	StartingThirst = 80.f;

	Health = StartingHealth;
	Hunger = StartingHunger;
	Stamina = StartingStamina;
	Thirst = StartingThirst;
	
	bIsHealing = true;
	HealPerSecond = 1.0f;

	bIsRegainingStamina = true;
	StaminaRecoveryPerSecond = 1.0f;
	StaminaDrainPerSecond = -0.5f;

	FoodDropPerSecond = -1.0f;
	WaterDropPerSecond = -0.01f;

	// Amount of gap between timers being fired.  0.05f is 20 times per second
	HealTimerFrequency = 0.05f;
	StaminaTimerFrequency = 0.05f;
	FoodTimerFrequency = 0.1f;
	WaterTimerFrequency = 0.1f;

	PlayerCharacter = nullptr;
	PlayerWidgetRef = nullptr;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASurvivalGameCharacter>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UStatComponent::Heal, HealTimerFrequency, true, HealTimerFrequency);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UStatComponent::AdjustStamina, StaminaTimerFrequency, true, StaminaTimerFrequency);
	GetWorld()->GetTimerManager().SetTimer(FoodTimerHandle, this, &UStatComponent::FoodDrain, FoodTimerFrequency, true, FoodTimerFrequency);
	GetWorld()->GetTimerManager().SetTimer(WaterTimerHandle, this, &UStatComponent::WaterDrain, WaterTimerFrequency, true, WaterTimerFrequency);
}

void UStatComponent::AdjustStat(float& StatToAdjust, float AmountToAdjust, float MaxValue, bool IsDamaging, float DamageAmount, UProgressBar* WidgetToUpdate)
{
	const float NewStatValue = FMath::Clamp(StatToAdjust + AmountToAdjust, 0.f, MaxValue);
	StatToAdjust = NewStatValue;

	if (PlayerWidgetRef && WidgetToUpdate)
	{
		WidgetToUpdate->SetPercent(StatToAdjust / MaxValue);
		//UpdateStatBar(StatToAdjust, MaxValue, WidgetToUpdate);
	}
	
	// Check if a stat being less than 0 is damaging to the player ie, too hungry but not run out of stamina
	if (NewStatValue <= 0.f && IsDamaging)
	{
		CauseDamage(DamageAmount);
	}
}

void UStatComponent::UpdateStatBar(float StatValue, float MaxValue, UProgressBar* WidgetToUpdate)
{
	if (WidgetToUpdate)
	{
		WidgetToUpdate->SetPercent(StatValue / MaxValue);
	}
}

void UStatComponent::SetPlayerWidgetRef(UPlayerWidget* ReferenceIn)
{
	if (ReferenceIn)
	{
		PlayerWidgetRef = ReferenceIn;
	}
}

void UStatComponent::CauseDamage(float DamageAmount)
{
	bIsHealing = false;
	AdjustStat(Health, DamageAmount * -1.0f, 100.f, false, 0.f, PlayerWidgetRef->GetStatsWidget()->GetHealthBar());

	if (PlayerCharacter)
	{
		// Check if the player has died from the damage
		if (Health <= 0.f)
		{
			PlayerCharacter->PlayerHasDied();
		}
		else
		{
			// Play the damage animation
			if (PlayerWidgetRef)
			{
				PlayerWidgetRef->PlayDamageAnim();
			}
		}
	}
	
}

void UStatComponent::Heal()
{
	if (bIsHealing && PlayerWidgetRef)
	{
		AdjustStat(Health, HealPerSecond * HealTimerFrequency, 100.f, false, 0.f, PlayerWidgetRef->GetStatsWidget()->GetHealthBar());
	}
}

void UStatComponent::AdjustStamina()
{
	if (PlayerWidgetRef)
	{
		if (bIsRegainingStamina)
		{
			AdjustStat(Stamina, StaminaRecoveryPerSecond * StaminaTimerFrequency, 100.f, false, 0.f, PlayerWidgetRef->GetStatsWidget()->GetStaminaBar());
		}
		else
		{
			AdjustStat(Stamina, StaminaDrainPerSecond * StaminaTimerFrequency, 100.f, false, 0.f, PlayerWidgetRef->GetStatsWidget()->GetStaminaBar());
			// Check if the player has run out of stamina
			if (Stamina <= 0.5f)
			{
				PlayerCharacter->StaminaHasRunOut();
			}
		}
	}
}

void UStatComponent::FoodDrain()
{
	if (PlayerWidgetRef)
	{
		AdjustStat(Hunger, FoodDropPerSecond * FoodTimerFrequency, 100.f, true, 0.05f, PlayerWidgetRef->GetStatsWidget()->GetFoodBar());
	}
}

void UStatComponent::WaterDrain()
{
	if (PlayerWidgetRef)
	{
		AdjustStat(Thirst, WaterDropPerSecond * WaterTimerFrequency, 100.f, true, 0.025f, PlayerWidgetRef->GetStatsWidget()->GetWaterBar());
	}
}

void UStatComponent::SetIsRegainingStamina(const bool NewStaminaState)
{
	bIsRegainingStamina = NewStaminaState;
}

