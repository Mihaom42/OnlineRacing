// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn/VMovementComponentReplicator.h"

// Sets default values for this component's properties
UVMovementComponentReplicator::UVMovementComponentReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVMovementComponentReplicator::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
	MoveComponent = GetOwner()->FindComponentByClass<UVMovementComponent>();
}

// Called every frame
void UVMovementComponentReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MoveComponent == nullptr)
	{
		return;
	}

	FVehicleMovement LastMove = MoveComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}
}

void UVMovementComponentReplicator::UpdateServerState(const FVehicleMovement& Move)
{
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MoveComponent->GetVelocity();
}

void UVMovementComponentReplicator::Server_SendMove_Implementation(FVehicleMovement Move)
{
	if (MoveComponent == nullptr)
	{
		return;
	}

	ClientSimulatedTime += Move.DeltaTime;

	MoveComponent->SimulateMove(Move);

	//UpdateServerState(Move);
}

bool UVMovementComponentReplicator::Server_SendMove_Validate(FVehicleMovement Move)
{
	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;

	if (!ClientNotRunningAhead)
	{
		UE_LOG(LogTemp, Error, TEXT("Client is running too fast."));
		return false;
	}
	if (!Move.IsValidMove())
	{
		UE_LOG(LogTemp, Error, TEXT("Recieved invalid move."));
		return false;
	}

	return true;
}