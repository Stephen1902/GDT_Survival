// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceBaseActor.generated.h"

UCLASS()
class SURVIVALGAME_API AResourceBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// Array of item(s) to be given when this object is interacted with
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	TArray<FDataTableRowHandle> ItemsToGive;

	// Default Health of this resource
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	float ResourceHealth;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
