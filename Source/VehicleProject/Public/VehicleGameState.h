// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "VehicleGameState.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEPROJECT_API AVehicleGameState : public AGameState
{
	GENERATED_BODY()
	
protected:

	bool bFirstFinished;
	bool bSecondFinished;
	bool bGameover{ false };
};
