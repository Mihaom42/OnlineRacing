// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "VehicleMovementComponent.generated.h"

USTRUCT()
struct FVehicleMove
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

	//bool IsValidMove() const { return FMath::Abs(Throttle) <= 1.0f && FMath::Abs(SteeringThrow) <= 1; };
};

USTRUCT()
struct FVehicleState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVehicleMove LastMove;
};

UCLASS()
class VEHICLEPROJECT_API UVehicleMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UVehicleMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//virtual void BeginPlay() override;

private:
	FVector Acceleration = FVector(0.f);

	TArray<FVehicleMove> UnacknowledgedMoves;

	UFUNCTION()
	void NetUpdateFunc();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FVehicleMove Move);
};
