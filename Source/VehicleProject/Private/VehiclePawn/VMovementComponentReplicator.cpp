// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn/VMovementComponentReplicator.h"
#include "Net/UnrealNetwork.h"

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

	UpdateServerState(Move);
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

void UVMovementComponentReplicator::OnRep_ServerState()
{
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRep_ServerState();
		break;
	default:
		break;
	}
}

void UVMovementComponentReplicator::SimulatedProxy_OnRep_ServerState()
{
	if (MoveComponent == nullptr)
	{
		return;
	}

	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;

	ClientTimeSinceUpdate = 0;

	if (MeshOffsetRoot != nullptr)
	{
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}

	ClientStartVelocity = MoveComponent->GetVelocity();

	GetOwner()->SetActorTransform(ServerState.Transform);
}

void UVMovementComponentReplicator::AutonomousProxy_OnRep_ServerState()
{
	if (MoveComponent == nullptr)
	{
		return;
	}

	GetOwner()->SetActorTransform(ServerState.Transform);

	MoveComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMovement& Move : UnacknowledgedMoves)
	{
		MoveComponent->SimulateMove(Move);
	}
}

void UVMovementComponentReplicator::ClearAcknowledgedMoves(FVehicleMovement LastMove)
{
	TArray<FVehicleMovement> NewMoves;

	for (const FVehicleMovement& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time) NewMoves.Add(Move);
	}

	UnacknowledgedMoves = NewMoves;
}

void UVMovementComponentReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVMovementComponentReplicator, ServerState);
}