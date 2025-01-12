// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DayNightSceneComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UDayNightSceneComp : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDayNightSceneComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Day Night Scene Comp", meta=(ClampMin=1.0f))
	float DaySpeedRotation;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddHours(const float HoursToAdd);
private:
	UPROPERTY()
	AActor* Owner;

	float SunRotation;
};
