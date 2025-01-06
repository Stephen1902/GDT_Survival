// Copyright 2024 DME Games


#include "AnimalBaseClass.h"

#include "AnimalAIController.h"
#include "ItemBaseActor.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAnimalBaseClass::AAnimalBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AAnimalAIController> FoundController(TEXT("'/Script/SurvivalGame.AnimalAIController'"));
	if (FoundController.Succeeded())
	{
		AIControllerClass = FoundController.Class;
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	WanderWalkSpeed = 350.f;
	ChaseWalkSpeed = 500.f;
	bIsDead = false;
	Health = 50.f;
	MinMeatToGive = 1;
	MaxMeatToGive = 5;
	bCanAttack = true;
	TimeBetweenAttacks = 1.0f;
}

// Called when the game starts or when spawned
void AAnimalBaseClass::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WanderWalkSpeed;

	OnTakeAnyDamage.AddDynamic(this, &AAnimalBaseClass::OnDamageReceived);
/*
	FTimerHandle TempTimer;
	GetWorld()->GetTimerManager().SetTimer(TempTimer, this, &AAnimalBaseClass::IsDead, 3.0f);
	*/
}

// Called every frame
void AAnimalBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AAnimalBaseClass::SetWalkSpeed(bool IsChasing)
{
	if (IsChasing)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChaseWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WanderWalkSpeed;
	}
}

void AAnimalBaseClass::CheckCanAttack()
{
	if (!bIsDead && bCanAttack)
	{
		AActor* HitActor = DoLineTrace();
		if (HitActor != nullptr)
		{
			const FDamageEvent DamageEvent;
			HitActor->TakeDamage(DamageCaused, DamageEvent, GetController(), this);
			if (ACharacter* Character = Cast<ACharacter>(HitActor))
			{
				FVector AmountToLaunch = GetActorForwardVector() * 500.f;
				AmountToLaunch.Z += 50.f;
				Character->LaunchCharacter(AmountToLaunch, false, false);
			}
		}

		if (AttackMontageToPlay)
		{
			PlayAnimMontage(AttackMontageToPlay);
		}

		if (AttackSoundToPlay)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSoundToPlay, GetActorLocation());
		}

		if (TimeBetweenAttacks > 0.f)
		{
			bCanAttack = false;
			GetWorld()->GetTimerManager().SetTimer(CanAttackTimerHandle, this, &AAnimalBaseClass::ResetCanAttack, TimeBetweenAttacks);
		}
	}
}

AActor* AAnimalBaseClass::DoLineTrace()
{
	FHitResult HitResult;
	const FVector StartLoc = GetActorLocation();
	const FVector EndLoc = (GetActorForwardVector() * 250.f) + StartLoc; 
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes;
	TraceTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoc, EndLoc, 56.f, TraceTypes, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was hit."), *HitResult.GetActor()->GetName());
		return HitResult.GetActor();
	}
	
	return nullptr;
}

void AAnimalBaseClass::IsDead()
{
	bIsDead = true;
	GetCharacterMovement()->DisableMovement();
	//GetMesh()->SetSimulatePhysics(true);

	if (MeatBPToSpawn)
	{
		int32 RandomAmountToGive = FMath::RandRange(MinMeatToGive, MaxMeatToGive);
		for (int32 i = 0; i < RandomAmountToGive; ++i)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Create a random location around the dead animal to spawn the meat
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.X += FMath::RandRange(-250.f, 250.f);
			SpawnLocation.Y += FMath::RandRange(-250.f, 250.f);
			SpawnLocation.Z += 50.f;

			// Create a random rotation
			FRotator SpawnRotator = FRotator::ZeroRotator;
			SpawnRotator.Pitch = FMath::RandRange(-90.f, 90.f);
			SpawnRotator.Roll = FMath::RandRange(-90.f, 90.f);

			// Spawn the item and tell it that it was spawned (not placed) in the world to simulate physics while new
			AItemBaseActor* NewItem = GetWorld()->SpawnActor<AItemBaseActor>(MeatBPToSpawn, SpawnLocation, SpawnRotator, SpawnParameters);
			NewItem->SetItemSpawnedBoolean(true);
		}
	}
	Destroy();
}

void AAnimalBaseClass::ResetCanAttack()
{
	bCanAttack = true;
}

void AAnimalBaseClass::OnDamageReceived(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Animal Damage Received."));
	Health -= Damage;
	if (Health <= 0.f)
	{
		IsDead();
	}
	else
	{
		if (HitSoundToPlay)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundToPlay, GetActorLocation());
		}

		// After a small delay, animal can attack again
		GetWorld()->GetTimerManager().SetTimer(CanAttackTimerHandle, this, &AAnimalBaseClass::ResetCanAttack, 0.1f);
	}
}
