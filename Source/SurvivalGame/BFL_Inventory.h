// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Inventory.generated.h"

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

	FInventoryStruct()
	{
		ItemName = "";
		Amount = 0;
		DisplayMesh = nullptr;
		Icon = nullptr;
		bCanCraft = false;
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
