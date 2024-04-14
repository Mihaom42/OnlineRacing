// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleHUDWidget.h"
#include "Components/TextBlock.h"
#include "UI/GameOverWidget.h"

UVehicleHUDWidget::UVehicleHUDWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UVehicleHUDWidget::NativeConstruct()
{
	if (GameOverWidget != nullptr)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVehicleHUDWidget::UpdateSpeedValue(FText Value)
{
	SpeedTextValue->SetText(Value);
}

void UVehicleHUDWidget::UpdateCountdownValue(int32 Value)
{
	if (Value < 1)
	{
		CountdownTextValue->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (Value == 3 && !CountdownTextValue->IsVisible())
	{
		CountdownTextValue->SetVisibility(ESlateVisibility::Visible);
	}

	CountdownTextValue->SetText(FText::FromString(FString::Printf(TEXT("%i"), Value)));
}

void UVehicleHUDWidget::UpdatePlayerPlace(FText Value)
{
	PlaceTextValue->SetText(Value);
}

void UVehicleHUDWidget::ShowGameOver()
{
	if (GameOverWidget != nullptr)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::Visible);
		PlaceTextValue->SetVisibility(ESlateVisibility::Hidden);
		SpeedTextValue->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVehicleHUDWidget::HideGameOver()
{
	if (GameOverWidget != nullptr)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
		PlaceTextValue->SetVisibility(ESlateVisibility::Visible);
		SpeedTextValue->SetVisibility(ESlateVisibility::Visible);
		//CountdownTextValue->SetVisibility(ESlateVisibility::Visible);
	}
}