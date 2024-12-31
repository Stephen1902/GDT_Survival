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
	PreviewMaterial = nullptr;
	MeshComponentToAdd = nullptr;
}

// Called when the game starts
void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInBuildMode && MeshComponentToAdd)
	{
		FVector LineTraceLocation;
		FRotator LineTraceRotation = FRotator::ZeroRotator;

		if (DoLineTrace(LineTraceLocation))
		{
			MeshComponentToAdd->SetWorldLocationAndRotation(LineTraceLocation, PlayerRef->GetActorRotation());
		}

		if (DoLineTraceSocket(LineTraceLocation, LineTraceRotation))
		{
			MeshComponentToAdd->SetWorldLocationAndRotation(LineTraceLocation, LineTraceRotation);
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

void UBuildingComponent::EnterBuildMode(FInventoryStruct* ItemToBuildWith)
{
	if (ItemToBuildWith)
	{
		// Check if an item isn't already trying to be built
		if (bIsInBuildMode && ItemToBuildWith != ItemToBuild)
		{
			EndBuildMode();
		}

		if (!PlayerRef)
		{
			PlayerRef = Cast<ASurvivalGameCharacter>(GetOwner());
		}
	
		if (PlayerRef)
		{
			ItemToBuild = ItemToBuildWith; 
			bIsInBuildMode = true;

			FTransform ComponentTransform;
			MeshComponentToAdd = Cast<UStaticMeshComponent>(PlayerRef->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, ComponentTransform, false));
			MeshComponentToAdd->SetStaticMesh(ItemToBuildWith->DisplayMesh);
			MeshComponentToAdd->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (PreviewMaterial)
			{
				MeshComponentToAdd->SetMaterial(0, PreviewMaterial);
			}

			TypeToTraceFor = ItemToBuildWith->BuildingStruct.SocketTraceType;
		}
		else
		{
			EndBuildMode();
		}
	}
}

void UBuildingComponent::EndBuildMode()
{
	bIsInBuildMode = false;
	ItemToBuild = nullptr;
	if (MeshComponentToAdd)
	{
		MeshComponentToAdd->SetStaticMesh(nullptr);
		MeshComponentToAdd = nullptr;
	}
}

void UBuildingComponent::PlaceItemInWorld()
{
	if (ItemToBuild)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABuildingBaseClass* NewSpawn = GetWorld()->SpawnActor<ABuildingBaseClass>(ItemToBuild->BuildingStruct.ClassToSpawn, MeshComponentToAdd->GetComponentTransform(), SpawnParameters);
		NewSpawn->SetDisplayMesh(ItemToBuild->DisplayMesh);

		if (PlayerRef)
		{
			// Remove an item from the inventory
			PlayerRef->GetInventoryComp()->RemoveItem(ItemToBuild->ItemName, 1);
			// Check if the player has more of these items in the inventory, to allow for multiple placing
			if (!PlayerRef->GetInventoryComp()->CheckItemExists(ItemToBuild))
			{
				EndBuildMode();
			}
		}
	}
	else
	{
		EndBuildMode();	
	}
}

bool UBuildingComponent::DoLineTrace(FVector& HitLocationOut)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector StartLoc = PCM->GetCameraLocation();
	FVector EndLoc = (PCM->GetActorForwardVector() * 1000.f) + StartLoc;

	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);
	
	if (HitResult.bBlockingHit)
	{
		HitLocationOut = HitResult.Location;
		return true;
	}
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, TEXT("No Valid Hit Location"));
	HitLocationOut = FVector(0.f);
	return false;
}

bool UBuildingComponent::DoLineTraceSocket(FVector& HitLocationOUT, FRotator& HitRotationOUT)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector StartLoc = PCM->GetCameraLocation();
	FVector EndLoc = (PCM->GetActorForwardVector() * 1000.f) + StartLoc;

	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, TypeToTraceFor, false, IgnoredActors,EDrawDebugTrace::None, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		HitResult.Component->GetSocketWorldLocationAndRotation("", HitLocationOUT, HitRotationOUT);
		return true;
	}

	HitLocationOUT = FVector(0.f);
	return false;
	
}

