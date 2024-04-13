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
	
public:
	AVehicleGameState();

	bool IsGameOver() { return bGameOver; }
	void MarkFinishPlayer(FString PlayerName);

protected:
	bool bGameOver;
	bool bFirstFinished;
	bool bSecondFinished;
};
