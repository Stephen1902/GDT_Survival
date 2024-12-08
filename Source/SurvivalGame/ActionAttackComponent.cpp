// Copyright 2024 DME Games


#include "ActionAttackComponent.h"

// Sets default values for this component's properties
UActionAttackComponent::UActionAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UActionAttackComponent::SetReferences(ASurvivalGameCharacter* SurvivalGameCharacter, UInventoryComponent* InventoryComponent)
{
	CharacterRef = SurvivalGameCharacter;
	InventoryComponentRef = InventoryComponent;
}

