// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Building Component")
	UMaterial* PreviewMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Building Component")
	bool bIsInBuildMode;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPlayerCharacterRef(class ASurvivalGameCharacter* PlayerRefIn);	
	void EnterBuildMode(struct FInventoryStruct* ItemToBuildWith);
	void EndBuildMode();

	bool GetIsInBuildMode() const { return bIsInBuildMode; }
	void PlaceItemInWorld();

private:
	UPROPERTY()
	ASurvivalGameCharacter* PlayerRef;

	FInventoryStruct* ItemToBuild;
	
	UPROPERTY()
	UStaticMeshComponent* MeshComponentToAdd;
	
	bool DoLineTrace(FVector& HitLocationOut);
	bool DoLineTraceSocket(FVector& HitLocationOUT, FRotator& HitRotationOUT);
	TEnumAsByte<ETraceTypeQuery> TypeToTraceFor;

};
