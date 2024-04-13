// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleHUDWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class VEHICLEPROJECT_API UVehicleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UVehicleHUDWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;

	void UpdateSpeedValue(FText Value);
	void UpdateCountdownValue(FText Value);
	void UpdatePlayerPlace(FText Value);

	void StartCountdown();
	int32 GetCountdownValue() { return Seconds; }

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SpeedTextValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountdownTextValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlaceTextValue;

private:
	FTimerHandle GameStartTimer;
	int32 Seconds;
};
