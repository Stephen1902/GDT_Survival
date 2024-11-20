// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BFL_Inventory.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryStruct> InventoryInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventoryWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	UStaticMesh* TempMesh;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(FInventoryStruct ItemToAdd);
	bool RemoveItem(FString ItemToRemove, int32 AmountToRemove);

	void DealWithInventoryButtonPress(APlayerController* PlayerControllerIn);
	void SetEquippedItem(FInventoryStruct* SlotItem);
private:
	UPROPERTY()
	UInventoryWidget* InventoryWidgetRef;

	FInventoryStruct* CurrentSetItem;
};
