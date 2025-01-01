// Copyright 2024 DME Games

#include "AnimalAIController.h"
#include "AnimalBaseClass.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/PawnSensingComponent.h"

AAnimalAIController::AAnimalAIController()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensingComponent->bHearNoises = false;
	PawnSensingComponent->HearingThreshold = 0.f;
	PawnSensingComponent->LOSHearingThreshold = 0.f;
	PawnSensingComponent->HearingMaxSoundAge = 0.f;
	PawnSensingComponent->SightRadius = 2500.f;
	PawnSensingComponent->SetPeripheralVisionAngle(90.f);
	
	ControlledChar = nullptr;
	bIsChasing = false;
	ActorToChase = nullptr;
}

void AAnimalAIController::BeginPlay()
{
	Super::BeginPlay();

	ControlledChar = Cast<AAnimalBaseClass>(GetPawn());
	GetWorld()->GetTimerManager().SetTimer(WanderTimer, this, &AAnimalAIController::Wander, 2.f, false);

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAnimalAIController::OnPawnSeen);
}

void AAnimalAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!bIsChasing)
	{
		const float RandomMoveTime = FMath::FRandRange(0.5f, 2.0f);
		GetWorld()->GetTimerManager().SetTimer(WanderTimer, this, &AAnimalAIController::Wander, RandomMoveTime, false);
	}
	else
	{
		ControlledChar->CheckCanAttack();
		//Chase(ActorToChase);
	}
}

void AAnimalAIController::Wander()
{
	GetWorld()->GetTimerManager().ClearTimer(WanderTimer);

	if (ControlledChar)
	{		
		const auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FNavLocation RandomMoveToLoc;
		if (NavSystem->GetRandomPointInNavigableRadius(ControlledChar->GetActorLocation(), 3500.f, RandomMoveToLoc))
		{
			MoveToLocation(RandomMoveToLoc.Location, 350.f);			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to move but Controlled Char is not valid."));
	}
}

void AAnimalAIController::Chase(AActor* TargetActor)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(WanderTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(WanderTimer);
	}
	
	if (TargetActor && ControlledChar)
	{
		if (ActorToChase == nullptr)
		{
			ActorToChase = TargetActor;
			ControlledChar->SetWalkSpeed(true);
			bIsChasing = true;
		}
				
		const auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FNavLocation RandomMoveToLoc;
		if (NavSystem->GetRandomPointInNavigableRadius(ControlledChar->GetActorLocation(), 3500.f, RandomMoveToLoc))
		{
			MoveToLocation(ActorToChase->GetActorLocation(), 150.f);			
		}		
	}
}

void AAnimalAIController::LostChaseTarget()
{
	if (ControlledChar)
	{
		ControlledChar->SetWalkSpeed(false);
	}
	
	ActorToChase = nullptr;
}

void AAnimalAIController::OnPawnSeen(APawn* SeenPawn)
{
	Chase(SeenPawn);
}
