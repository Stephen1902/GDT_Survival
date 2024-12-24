// Copyright 2024 DME Games

#include "BuildingComponent.h"

#include "BuildingBaseClass.h"
#include "SurvivalGameCharacter.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBuildingComponent::UBuildingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsInBuildMode = false;
	PlayerRef = nullptr;
	BuildTable = nullptr;
	PreviewMaterial = nullptr;
	MeshComponentToAdd = nullptr;
}


// Called when the game starts
void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BuildTable)
	{
		BuildPart.DataTable = BuildTable;
		EnterBuildMode();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Build Table is not set in Player BuildingComponent."));
	}
	
}

// Called every frame
void UBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInBuildMode && MeshComponentToAdd)
	{
		FVector LineTractHit;
		if (DoLineTrace(LineTractHit))
		{
			MeshComponentToAdd->SetWorldLocationAndRotation(LineTractHit, PlayerRef->GetActorRotation());
		}
	}
}

void UBuildingComponent::SetPlayerCharacterRef(ASurvivalGameCharacter* PlayerRefIn)
{
	if (PlayerRefIn != nullptr)
	{
		PlayerRef = PlayerRefIn;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPlayerRef called in BuildingComponent but PlayerRefIn was not valid."));
	}
}

void UBuildingComponent::EnterBuildMode()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<ASurvivalGameCharacter>(GetOwner());
	}

	if (PlayerRef && BuildTable)
	{
		bIsInBuildMode = true;

		
		BuildPart.RowName = "Floor"; //FName(PlayerRef->GetInventoryComp()->GetCurrentEquippedItem()->ItemName);

		if (FBuildingStruct* Row = BuildTable->FindRow<FBuildingStruct>(BuildPart.RowName, ""))
		{
			FTransform ComponentTransform;
			MeshComponentToAdd = Cast<UStaticMeshComponent>(PlayerRef->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, ComponentTransform, false));
			MeshComponentToAdd->SetStaticMesh(Row->DisplayMesh);
			MeshComponentToAdd->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (PreviewMaterial)
			{
				MeshComponentToAdd->SetMaterial(0, PreviewMaterial);
			}
		}
	}
	else
	{
		EndBuildMode();
	}
}

void UBuildingComponent::EndBuildMode()
{
	bIsInBuildMode = false;
	BuildPart.RowName = FName("");
	if (MeshComponentToAdd)
	{
		MeshComponentToAdd->SetStaticMesh(nullptr);
		MeshComponentToAdd = nullptr;
	}
}

void UBuildingComponent::PlaceItemInWorld()
{
	if (FBuildingStruct* Row = BuildTable->FindRow<FBuildingStruct>(BuildPart.RowName, ""))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABuildingBaseClass* NewSpawn = GetWorld()->SpawnActor<ABuildingBaseClass>(Row->ClassToSpawn, MeshComponentToAdd->GetComponentTransform(), SpawnParameters);
		NewSpawn->SetDisplayMesh(Row->DisplayMesh);
	}
	
	EndBuildMode();
}

bool UBuildingComponent::DoLineTrace(FVector& HitLocationOut)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector StartLoc = PCM->GetCameraLocation();
	FVector EndLoc = (PCM->GetActorForwardVector() * 1000.f) + StartLoc;

	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, TraceTypeQuery1, false, IgnoredActors,EDrawDebugTrace::None, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		HitLocationOut = HitResult.Location;
		return true;
	}

	HitLocationOut = FVector(0.f);
	return false;
}

