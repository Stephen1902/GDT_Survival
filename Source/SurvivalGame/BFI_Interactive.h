// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BFL_Inventory.h"
#include "BFI_Interactive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBFI_Interactive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVALGAME_API IBFI_Interactive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact|ToInteract")
	void OnInteract(FInventoryStruct& ItemToInteractWith, bool& bCanPickUp);
};
