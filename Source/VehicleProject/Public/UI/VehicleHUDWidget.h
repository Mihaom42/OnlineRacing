// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleHUDWidget.generated.h"

class UTextBlock;
class UButton;
class UGameOverWidget;

UCLASS()
class VEHICLEPROJECT_API UVehicleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UVehicleHUDWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;

	void UpdateSpeedValue(FText Value);
	void UpdateCountdownValue(int32 Value);
	void UpdatePlayerPlace(FText Value);

	void ShowGameOver();
	void HideGameOver();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SpeedTextValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountdownTextValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlaceTextValue;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UGameOverWidget* GameOverWidget;
};
