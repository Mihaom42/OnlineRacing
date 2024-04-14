// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn/VehiclePlayerState.h"
#include "VehicleGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AVehiclePlayerState::AVehiclePlayerState()
{
	bPlayAgain = false;
}

void AVehiclePlayerState::SetPlayAgain_Implementation(bool bInPlayAgain)
{
	bPlayAgain = bInPlayAgain;

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (GameState)
	{
		AVehicleGameState* VehicleGameState = Cast<AVehicleGameState>(GameState);
		if (VehicleGameState)
		{
			VehicleGameState->RestartGame();
		}
	}
}

void AVehiclePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehiclePlayerState, bPlayAgain)
}
