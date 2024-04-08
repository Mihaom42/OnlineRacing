// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehicleController.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEPROJECT_API AVehicleController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AVehicleController();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
