// Copyright 2024 DME Games

#pragma once

#include "BFL_Inventory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBaseActor.generated.h"

struct FInventoryStruct;

UCLASS()
class SURVIVALGAME_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
#if WITH_EDITOR
	AItemBaseActor();
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	TObjectPtr<class USphereComponent> OverlapComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	FInventoryStruct InventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	FDataTableRowHandle ItemTableRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	bool bIsRandomItem;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
