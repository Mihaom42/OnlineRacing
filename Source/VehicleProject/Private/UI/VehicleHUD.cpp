// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleHUD.h"
#include "UI/VehicleHUDWidget.h"
#include "VehiclePawn/Vehicle.h"
#include "Kismet/GameplayStatics.h"
#include "VehicleGameState.h"

void AVehicleHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	if (HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UVehicleHUDWidget>(World, HUDWidgetClass);
	}

	if (HUDWidget != nullptr)
	{
		HUDWidget->AddToViewport();
	}

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);

	if (GameState != nullptr)
	{
		VehicleGameState = Cast<AVehicleGameState>(GameState);
	}
}

void AVehicleHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//TODO: Maybe it is worth to find another way to update data 
void AVehicleHUD::DrawHUD()
{
	Super::DrawHUD();

	AVehicle* Vehicle = Cast<AVehicle>(GetOwningPawn());

	if (Vehicle != nullptr && HUDWidget != nullptr)
	{
		HUDWidget->UpdateSpeedValue(Vehicle->GetSpeed());
		HUDWidget->UpdatePlayerPlace(FText::AsNumber(Vehicle->GetPlayerPlace()));
	}

	if (VehicleGameState != nullptr)
	{
		if (HUDWidget != nullptr)
		{
			HUDWidget->UpdateCountdownValue(VehicleGameState->GetCountdownTimeValue());
		}

		if (VehicleGameState->IsGameOver())
		{
			HUDWidget->ShowGameOver();
		}
		else
		{
			HUDWidget->HideGameOver();
		}
	}
}