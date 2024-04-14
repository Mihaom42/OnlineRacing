// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VehicleGameMode.generated.h"

UCLASS()
class VEHICLEPROJECT_API AVehicleGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVehicleGameMode();

	void StartGame() const;
	void EndGame();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
