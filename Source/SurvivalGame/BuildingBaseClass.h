// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBaseClass.generated.h"

UCLASS()
class SURVIVALGAME_API ABuildingBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Base")
	USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Base")
	UStaticMeshComponent* MeshToDisplay;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDisplayMesh(UStaticMesh* MeshIn) const;
};
