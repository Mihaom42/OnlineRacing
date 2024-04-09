// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn/VMovementComponent.h"
#include "Engine/World.h" 


// Sets default values for this component's properties
UVMovementComponent::UVMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UVMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy || GetOwnerRole() == ROLE_Authority)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UVMovementComponent::SimulateMove(const FVehicleMovement& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	// Applying the air drag to the Force
	Force += GetAirResistance();
	// Applying the rolling resistance to the Force
	Force += GetRollingResistance();
	// a = F / m
	FVector Acceleration = Force / Mass;
	// dv = a * dt
	Velocity += Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Move.Throttle: %f"), Move.Throttle));
}

FVehicleMovement UVMovementComponent::CreateMove(float DeltaTime)
{
	FVehicleMovement Move;

	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;

	return Move;
}

FVector UVMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoef;
}

FVector UVMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100.0f;
	float NormalForce = Mass * AccelerationDueToGravity;

	return -Velocity.GetSafeNormal() * RollingResistanceCoef * NormalForce;
}

void UVMovementComponent::ApplyRotation(float DeltaTime, float InSteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * InSteeringThrow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	// Rotating our velocity as we rotate the car
	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UVMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FHitResult Hit;
	// MPS * 100 = CMPS * S = Velocity in CM.
	FVector Translation = Velocity * 100 * DeltaTime;

	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);
	if (Hit.IsValidBlockingHit()) Velocity = FVector::ZeroVector;
}