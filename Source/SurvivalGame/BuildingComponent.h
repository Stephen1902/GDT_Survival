// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"

USTRUCT(BlueprintType)
struct FBuildingStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	FString ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<class ABuildingBaseClass> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UStaticMesh* DisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TEnumAsByte<ETraceTypeQuery> SocketTraceType;

	FBuildingStruct()
	{
		ItemName = "";
		ClassToSpawn = nullptr;
		DisplayMesh = nullptr;
		SocketTraceType = TraceTypeQuery1;
	}
};

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

	// Data Table to be used that contains the build part information
	UPROPERTY(EditAnywhere, Category = "Building Component")
	UDataTable* BuildTable;
	
	UPROPERTY(EditAnywhere, Category = "Building Component")
	UMaterial* PreviewMaterial;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPlayerCharacterRef(class ASurvivalGameCharacter* PlayerRefIn);	
	void EnterBuildMode();
	void EndBuildMode();

	bool GetIsInBuildMode() const { return bIsInBuildMode; }
	void PlaceItemInWorld();

private:
	UPROPERTY()
	ASurvivalGameCharacter* PlayerRef;

	FDataTableRowHandle BuildPart;

	UPROPERTY()
	UStaticMeshComponent* MeshComponentToAdd;

	bool bIsInBuildMode;

	bool DoLineTrace(FVector& HitLocationOut);
	bool DoLineTraceSocket(FVector& HitLocationOUT, FRotator& HitRotationOUT);
	TEnumAsByte<ETraceTypeQuery> TypeToTraceFor;

};
