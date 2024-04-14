// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleGameState.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "VehiclePawn/VehiclePlayerState.h"
#include "UI/VehicleHUD.h"
#include "Net/UnrealNetwork.h"

AVehicleGameState::AVehicleGameState()
{
	bFirstFinished = false;
	bSecondFinished = false;
	bGameOver = false;
}

void AVehicleGameState::MarkFinishPlayer(FString PlayerName)
{
	if (PlayerName == "FirstPlayer")
	{
		bFirstFinished = true;
	}
	else if (PlayerName == "SecondPlayer")
	{
		bSecondFinished = true;
	}

	if (bFirstFinished == true && bSecondFinished == true)
	{
		bGameOver = true;
	}
}

void AVehicleGameState::RestartGame_Implementation()
{
	int32 PlayersReady = 0;

	for (APlayerState* PlayerState : PlayerArray)
	{
		if (Cast<AVehiclePlayerState>(PlayerState)->GetPlayAgain())
		{
			PlayersReady++;
		}
	}

	if (PlayersReady == 2)
	{
		bFirstFinished = false;
		bSecondFinished = false;
		bGameOver = false;
	}
}

void AVehicleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehicleGameState, bGameOver);
	DOREPLIFETIME(AVehicleGameState, bFirstFinished);
	DOREPLIFETIME(AVehicleGameState, bSecondFinished);
}