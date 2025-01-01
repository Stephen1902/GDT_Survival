// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimalBaseClass.generated.h"

UCLASS()
class SURVIVALGAME_API AAnimalBaseClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimalBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The walk speed of this animal when it has no target to chase
	UPROPERTY(EditAnywhere, Category = "Animal Base Class")
	float WanderWalkSpeed;

	// The walk speed of this animal when it has a target and is chasing it
	UPROPERTY(EditAnywhere, Category = "Animal Base Class")
	float ChaseWalkSpeed;

	// The amount of damage this animal causes when it attacks
	UPROPERTY(EditAnywhere, Category = "Animal Base Class")
	float DamageCaused;

	// The montage to be played when this animal attacks
	UPROPERTY(EditAnywhere, Category = "Animal Base Class")
	UAnimMontage* AttackMontageToPlay;

	// The sound that is played when this animal attacks
	UPROPERTY(EditAnywhere, Category = "Animal Base Class")
	USoundBase* AttackSoundToPlay;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWalkSpeed(bool IsChasing);

	void CheckCanAttack();
	AActor* DoLineTrace();
};
