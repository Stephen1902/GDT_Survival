// Copyright 2024 DME Games

#pragma once

#include "BFL_Inventory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFI_Interactive.h"
#include "ItemBaseActor.generated.h"

struct FInventoryStruct;

UCLASS()
class SURVIVALGAME_API AItemBaseActor : public AActor, public IBFI_Interactive
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
	
	// Whether item is spawned randomly by the PCG
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	bool bIsRandom;

	// If random, which item(s) can be spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor", meta=(EditCondition="bIsRandom"))
	TMap<FName, float> RandomItems;	
	
	// Item(s) that this actor returns
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	TArray<FDataTableRowHandle> InventoryItem;

	// If true, item is collected and immediately destroyed.  If not, damage is caused to its health 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor")
	bool bItemIsPickedUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Actor", meta=(EditCondition="!bItemIsPickedUp"))
	float StartingHealth;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(float Damage, FInventoryStruct& ItemToInteractWith, APlayerController* Controller, AActor* DamageCauser) override;

	void SetItemSpawnedBoolean(bool ItemSpawnedIn);
private:
	float CurrentHealth;

	//UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* DamageInstigator, AActor* DamageCauser);

	bool bItemSpawnedAtRuntime = false;
	FTimerHandle RemovePhysicsHandle;
	void RemovePhysics();

	// Data Table used by this item
	UPROPERTY()
	UDataTable* InventoryDataTable;
	
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
