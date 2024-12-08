// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGameCharacter.h"
#include "Components/ActorComponent.h"
#include "ActionAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UActionAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Attack Component")
	TObjectPtr<class ASurvivalGameCharacter> CharacterRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Attack Component")
	TObjectPtr<class UInventoryComponent> InventoryComponentRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Attack Component")
	TObjectPtr<class UInventoryWidget> InventoryWidgetRef;

public:	
	void SetReferences(ASurvivalGameCharacter* SurvivalGameCharacter, UInventoryComponent* InventoryComponent);
	
};
