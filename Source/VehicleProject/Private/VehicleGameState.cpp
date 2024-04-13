// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleGameState.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"

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

	/*	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);

		if (GameMode != nullptr)
		{
			AVehicleGameMode* VehicleGameMode = Cast<AVehicleGameMode>(GameMode);

			if (VehicleGameMode != nullptr)
			{
				VehicleGameMode->EndGame();
			}
		}*/
	}
}
