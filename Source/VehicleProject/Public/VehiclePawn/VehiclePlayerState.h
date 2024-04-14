// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VehiclePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEPROJECT_API AVehiclePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AVehiclePlayerState();

	bool GetPlayAgain() const { return bPlayAgain; }

	UFUNCTION(Server, Reliable)
	void SetPlayAgain(bool bInPlayAgain);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	bool bPlayAgain;
};
