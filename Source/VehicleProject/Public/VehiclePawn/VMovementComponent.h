// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VMovementComponent.generated.h"

USTRUCT()
struct FVehicleMovement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLEPROJECT_API UVMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void SimulateMove(const FVehicleMovement& Move);

	FVector GetVelocity() { return Velocity; }

	void SetVelocity(FVector Val) { Velocity = Val; }

	void SetThrottle(float Val) { Throttle = Val; }

	void SetSteeringThrow(float Val) { SteeringThrow = Val; }

	FVehicleMovement GetLastMove() { return LastMove; }

private:
	// The Mass of the car (kg). 1000kg = 1ton
	UPROPERTY(EditAnywhere)
	float Mass = 1000.0f;

	// The force applied to the car when the throttle is fully down (N)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000.0f;

	// Minimum radius of the car turning circle at full lock (m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10.0f;

	// Higher means more drag. 
	UPROPERTY(EditAnywhere)
	float DragCoef = 16.0f;

	// Higher means more Rolling Resistance.
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoef = 0.015;

	float Throttle;
	float SteeringThrow;

	FVehicleMovement LastMove;

	FVector Velocity;

	FVehicleMovement CreateMove(float DeltaTime);

	FVector GetAirResistance();

	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime, float InSteeringThrow);

	void UpdateLocationFromVelocity(float DeltaTime);
};
