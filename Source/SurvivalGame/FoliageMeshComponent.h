// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "BFI_Interactive.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "FoliageMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UFoliageMeshComponent : public UFoliageInstancedStaticMeshComponent, public IBFI_Interactive
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foliage|Health")
	float StartingHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foliage|Data Table")
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foliage|Sounds")
	USoundBase* TreeHittingSound;
	
private:
	UPROPERTY()
	TObjectPtr<class ASurvivalGameCharacter> PlayerCharacterRef;

	virtual void OnInteractFoliage_Implementation(int32 FoliageInteractedWith, float DamageCaused, FInventoryStruct& ItemToInteractWith) override;
	
	TArray<float> CurrentHealth;
};
