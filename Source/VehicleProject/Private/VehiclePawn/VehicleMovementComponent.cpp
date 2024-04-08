// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn/VehicleMovementComponent.h"
#include "Net/UnrealNetwork.h"

UVehicleMovementComponent::UVehicleMovementComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

//void UVehicleMovementComponent::BeginPlay()
//{
//	Super::BeginPlay();
//	//SetIsReplicated(true);
//}

void UVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime) /*|| (bIsActive == false)*/ || UpdatedComponent->IsSimulatingPhysics()) {
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector GravityAccel = FVector(0.f, 0.f, 0.f);
	FVector DragForce = 0.5f * 0.001f * (Velocity * Velocity.GetAbs());
	FVector DragAccel = DragForce / 1.0f; // Assume mass is 1
	FVector InputForce = ConsumeInputVector().GetClampedToMaxSize(1.0f) * 300.f;
	FVector InputAccel = InputForce / 1.0f;

	FVector NewAcceleration = InputAccel + GravityAccel - DragAccel;
	FVector NewVelocity = Velocity + (Acceleration + NewAcceleration) * (DeltaTime * 0.5f);
	FVector MoveDelta = NewVelocity * DeltaTime + NewAcceleration * DeltaTime * DeltaTime * 0.5f;

	FVector DesiredMovementThisFrame = MoveDelta;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		// Uses engine physics to move our Pawn Movement Component while respecting solid barriers
		FHitResult Hit(1.0f);
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
	}

	Velocity = NewVelocity;
	Acceleration = NewAcceleration;

	UpdateComponentVelocity();

	NetUpdateFunc();
}

void UVehicleMovementComponent::NetUpdateFunc()
{
	if (GetOwnerRole() == ROLE_Authority)
	{

	}
}

void UVehicleMovementComponent::Server_SendMove_Implementation(FVehicleMove Move)
{
}

bool UVehicleMovementComponent::Server_SendMove_Validate(FVehicleMove Move)
{
	return true;
}