// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleGameState.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "VehiclePawn/VehiclePlayerState.h"
#include "VehiclePawn/Vehicle.h"
#include "UI/VehicleHUD.h"
#include "Net/UnrealNetwork.h"
#include "VehiclePawn/VehicleController.h"

AVehicleGameState::AVehicleGameState()
{
	bFirstFinished = false;
	bSecondFinished = false;
	bGameOver = false;
	CountdownValue = 3;
}

void AVehicleGameState::BeginPlay()
{
	StartCountdown();
}

void AVehicleGameState::StartCountdown()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(GameStartTimer, this, &AVehicleGameState::DecreaseCountdownValue, 1.0f, true, 1.0f);
	}
}

void AVehicleGameState::DecreaseCountdownValue()
{
	if (CountdownValue >= 1)
	{
		CountdownValue--;
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().PauseTimer(GameStartTimer);
		}

		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
		if (GameMode != nullptr)
		{
			AVehicleGameMode* VehicleGameMode = Cast<AVehicleGameMode>(GameMode);
			if (VehicleGameMode != nullptr)
			{
				VehicleGameMode->StartGame();
			}
		}
	}
}

void AVehicleGameState::MarkFinishPlayer(FString PlayerName)
{
	if (PlayerArray.Num() == 1)
	{
		bGameOver = true;
		return;
	}

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

	if (PlayersReady == PlayerArray.Num())
	{
		bFirstFinished = false;
		bSecondFinished = false;
		bGameOver = false;
		CountdownValue = 3;

		for (APlayerState* PlayerState : PlayerArray)
		{
			AVehicleController* PC = Cast<AVehicleController>(PlayerState->GetPlayerController());
			if (PC)
			{
				PC->UnPossess();
			}

			Cast<AVehiclePlayerState>(PlayerState)->SetPlayAgain(false);
			ReturnPlayersToStartPoint();
			StartCountdown();
		}
	}
}

void AVehicleGameState::ReturnPlayersToStartPoint()
{
	TArray<AActor*> PawnArray;
	UGameplayStatics::GetAllActorsOfClass(this, AVehicle::StaticClass(), PawnArray);

	for (AActor* Actor : PawnArray)
	{
		AVehicle* VehiclePlayer = Cast<AVehicle>(Actor);

		if (VehiclePlayer != nullptr)
		{
			VehiclePlayer->SetActorLocation(VehiclePlayer->GetPlayerStartPosition());
			VehiclePlayer->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		}
	}
}

void AVehicleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehicleGameState, bGameOver);
	DOREPLIFETIME(AVehicleGameState, bFirstFinished);
	DOREPLIFETIME(AVehicleGameState, bSecondFinished);
	DOREPLIFETIME(AVehicleGameState, CountdownValue);
}