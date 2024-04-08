// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleGameMode.h"
#include "VehicleGameState.h"
#include "VehiclePawn/VehicleController.h"
#include "VehiclePawn/Vehicle.h"
#include "UI/VehicleHUD.h"
#include "Kismet/GameplayStatics.h"

AVehicleGameMode::AVehicleGameMode()
{
	DefaultPawnClass = AVehicle::StaticClass();
	PlayerControllerClass = AVehicleController::StaticClass();
	GameStateClass = AVehicleGameState::StaticClass();
	HUDClass = AVehicleHUD::StaticClass();
}

void AVehicleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//posses a pawn
	TArray<AActor*> PawnArray;
	UGameplayStatics::GetAllActorsOfClass(this, DefaultPawnClass, PawnArray);

	if (PawnArray.Num() > 0)
	{
		for (AActor* Actor : PawnArray)
		{
			AVehicle* VehiclePlayer = Cast<AVehicle>(Actor);
			if (VehiclePlayer != nullptr && !VehiclePlayer->IsPawnControlled())
			{
				NewPlayer->Possess(VehiclePlayer);
				VehiclePlayer->SetOwner(NewPlayer);
			}
		}
	}

	if (GetNumPlayers() == 2)
	{
		StartGame();
	}
}

void AVehicleGameMode::StartGame() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("StartGame")));
}

void AVehicleGameMode::EndGame()
{
}