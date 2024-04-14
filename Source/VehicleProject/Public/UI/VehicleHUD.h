// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VehicleHUD.generated.h"

class UVehicleHUDWidget;
class UGameOverWidget;

UCLASS()
class VEHICLEPROJECT_API AVehicleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void DrawHUD() override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Widgets)
	TSubclassOf<UVehicleHUDWidget> HUDWidgetClass;

private:
	UVehicleHUDWidget* HUDWidget;

	class AVehicleGameState* VehicleGameState;
};
