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

	TArray<AActor*> PawnArray;
	UGameplayStatics::GetAllActorsOfClass(this, AVehicle::StaticClass(), PawnArray);

	if (PawnArray.Num() > 0)
	{
		TArray<AActor*> ControllerArray;
		UGameplayStatics::GetAllActorsOfClass(this, AVehicleController::StaticClass(), ControllerArray);

		for (int Index = 0; Index < PawnArray.Num(); Index++)
		{
			AVehicle* VehiclePlayer = Cast<AVehicle>(PawnArray[Index]);

			if (VehiclePlayer != nullptr && !VehiclePlayer->IsPawnControlled())
			{
				AVehicleController* VehicleController = Cast<AVehicleController>(ControllerArray[Index]);

				if (VehicleController == nullptr)
				{
					return;
				}

				VehicleController->Possess(VehiclePlayer);
				VehiclePlayer->SetOwner(VehicleController);
			}
		}
	}
}

void AVehicleGameMode::StartGame() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("StartGame")));
}

void AVehicleGameMode::EndGame()
{
}