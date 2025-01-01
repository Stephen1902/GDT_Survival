// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AnimalAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()

	AAnimalAIController();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animal Contoller")
	class UPawnSensingComponent* PawnSensingComponent;
	
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

private:
	UPROPERTY()
	class AAnimalBaseClass* ControlledChar;

	FTimerHandle WanderTimer;

	void Wander();
	void Chase(AActor* TargetActor);
	void LostChaseTarget();

	UPROPERTY()
	AActor* ActorToChase;
	bool bIsChasing;

	bool bIsDead;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
};
