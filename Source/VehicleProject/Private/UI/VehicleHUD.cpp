// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleHUD.h"
#include "UI/VehicleHUDWidget.h"
#include "VehiclePawn/Vehicle.h"

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
}

void AVehicleHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AVehicleHUD::DrawHUD()
{
	Super::DrawHUD();

	AVehicle* Vehicle = Cast<AVehicle>(GetOwningPawn());

	if (Vehicle != nullptr && HUDWidget != nullptr)
	{
		HUDWidget->UpdateSpeedValue(Vehicle->SpeedString);
		//HUDWidget->UpdatePlayerPlace(FText::AsNumber(Vehicle->PlayerPlace));
	}

	if (HUDWidget != nullptr)
	{
		HUDWidget->UpdateCountdownValue(FText::FromString(FString::Printf(TEXT("%i"), HUDWidget->GetCountdownValue())));
	}
}
