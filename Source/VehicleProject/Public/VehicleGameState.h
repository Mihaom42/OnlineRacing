// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "VehicleGameState.generated.h"

class AVehicle;

UCLASS()
class VEHICLEPROJECT_API AVehicleGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AVehicleGameState();

	bool IsGameOver() { return bGameOver; }
	void MarkFinishPlayer(FString PlayerName);

	UFUNCTION(Server, Reliable)
	void RestartGame();

	void ReturnPlayersToStartPoint();

	int32 GetCountdownTimeValue() { return CountdownValue; }

	void StartCountdown();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	bool bGameOver;

	UPROPERTY(Replicated)
	bool bFirstFinished;

	UPROPERTY(Replicated)
	bool bSecondFinished;

	UPROPERTY(Replicated)
	int32 CountdownValue;

private:
	//UFUNCTION(Server, Reliable, BlueprintCallable)
	void DecreaseCountdownValue();

	FTimerHandle GameStartTimer;
};
