// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Inventory.generated.h"

USTRUCT(BlueprintType)
struct FCraftingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FDataTableRowHandle InventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta=(ClampMin=1))
	int32 NumberRequired;
};

USTRUCT(BlueprintType)
struct FBuildingStruct
{
	GENERATED_BODY()
	
	// Class to spawn into the world when this item is placed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<class ABuildingBaseClass> ClassToSpawn;

	// Trace to look for when this item is being placed into the world.  Used to join objects together.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TEnumAsByte<ETraceTypeQuery> SocketTraceType;

	FBuildingStruct()
	{
		ClassToSpawn = nullptr;
		SocketTraceType = TraceTypeQuery1;
	}
};

USTRUCT(BlueprintType)
struct FInventoryStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	FString ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UStaticMesh* DisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bCanCraft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta=(EditCondition="bCanCraft"))
	TArray<FCraftingItem> CraftingItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bCanBePlaced;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta=(EditCondition="bCanBePlaced"));
	FBuildingStruct BuildingStruct;
	
	FInventoryStruct()
	{
		ItemName = "";
		Amount = 0;
		DisplayMesh = nullptr;
		Icon = nullptr;
		bCanCraft = false;
		bCanBePlaced = false;
	}
};

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBFL_Inventory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
