// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehiclePawn/VMovementComponent.h"
#include "VMovementComponentReplicator.generated.h"

USTRUCT()
struct FVehicleState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVehicleMovement LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLEPROJECT_API UVMovementComponentReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVMovementComponentReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVMovementComponent* MoveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MeshOffsetRoot;

private:
	UPROPERTY(/*ReplicatedUsing = OnRep_ServerState*/)
	FVehicleState ServerState;

	/*UFUNCTION()
	void OnRep_ServerState();*/

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FVehicleMovement Move);

	void UpdateServerState(const FVehicleMovement& Move);

	TArray<FVehicleMovement> UnacknowledgedMoves;

	float ClientSimulatedTime;
};
