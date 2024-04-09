// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehiclePawn/VMovementComponentReplicator.h"
#include "VehiclePawn/VMovementComponent.h"
#include "Vehicle.generated.h"

UCLASS()
class VEHICLEPROJECT_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	AVehicle();

	virtual void Tick(float Delta) override;

	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CarMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class UVMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere)
	class UVMovementComponentReplicator* MovementReplicator;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	//void PushHandbrake(const FInputActionValue& Value);
};
