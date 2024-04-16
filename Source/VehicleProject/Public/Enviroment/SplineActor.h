// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

class USplineComponent;

UCLASS()
class VEHICLEPROJECT_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USplineComponent* NewSplineComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle UserTimer;

	FVector ClosestPoint;
	float DistanceToClosestPoint;
	float NormalizedDistance;
	float RemainingDistance;

	TArray<float> Distances;

	float GetDistanceAlongSplineForWorldLocation(FVector Location) const;
	void CollectRemainigDistance();
};
