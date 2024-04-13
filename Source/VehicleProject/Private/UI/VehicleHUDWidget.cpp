// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleHUDWidget.h"
#include "Components/TextBlock.h"

UVehicleHUDWidget::UVehicleHUDWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Seconds = 3;
}

void UVehicleHUDWidget::NativeConstruct()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(GameStartTimer, this, &UVehicleHUDWidget::StartCountdown, 1.0f, true, 1.0f);
	}
}

void UVehicleHUDWidget::UpdateSpeedValue(FText Value)
{
	SpeedTextValue->SetText(Value);
}

void UVehicleHUDWidget::UpdateCountdownValue(FText Value)
{
	CountdownTextValue->SetText(Value);
}

void UVehicleHUDWidget::UpdatePlayerPlace(FText Value)
{
	PlaceTextValue->SetText(Value);
}

void UVehicleHUDWidget::StartCountdown()
{
	if (Seconds > 1)
	{
		Seconds--;
	}
	else
	{
		CountdownTextValue->SetVisibility(ESlateVisibility::Hidden);
	}
}
