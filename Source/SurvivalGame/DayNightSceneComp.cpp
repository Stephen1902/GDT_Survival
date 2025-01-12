// Copyright 2024 DME Games


#include "DayNightSceneComp.h"

// Sets default values for this component's properties
UDayNightSceneComp::UDayNightSceneComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DaySpeedRotation = 5.0f;
	SunRotation = 0.f;
	Owner = nullptr;
}


// Called when the game starts
void UDayNightSceneComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	
}


// Called every frame
void UDayNightSceneComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner)
	{
		FRotator RotationThisTick = FRotator::ZeroRotator;
		RotationThisTick.Pitch = DaySpeedRotation * DeltaTime;
		SunRotation = RotationThisTick.Pitch;
		Owner->AddActorLocalRotation(RotationThisTick);
	}
}

void UDayNightSceneComp::AddHours(const float HoursToAdd)
{
	SunRotation += HoursToAdd;
}

