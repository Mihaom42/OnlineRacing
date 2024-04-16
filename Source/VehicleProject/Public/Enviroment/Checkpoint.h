// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class VEHICLEPROJECT_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
};
