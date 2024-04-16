// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/SplineActor.h"
#include "Components/SplineComponent.h" 
#include "VehiclePawn/Vehicle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

	NewSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("NewSplineComponent"));
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(UserTimer, this, &ASplineActor::CollectRemainigDistance, 0.2f, true, 0.1f);
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	NewSplineComponent->ClearSplinePoints(true);

	int32 SplinePoints = FMath::TruncToInt32(SplineComponent->GetSplineLength() / 100);

	for (int32 Index = 0; Index < SplinePoints; Index++)
	{
		NewSplineComponent->AddSplinePointAtIndex(SplineComponent->GetLocationAtDistanceAlongSpline(Index * 100, ESplineCoordinateSpace::World), Index, ESplineCoordinateSpace::Local, true);
	}	
}

void ASplineActor::Tick(float DeltaSeconds)
{
	//CollectRemainigDisance();
}

void ASplineActor::CollectRemainigDistance()
{
	TArray<AActor*> PawnArray;
	UGameplayStatics::GetAllActorsOfClass(this, AVehicle::StaticClass(), PawnArray);

	TArray<AVehicle*> VehicleArray;

	for (AActor* Pawn : PawnArray)
	{
		VehicleArray.Add(Cast<AVehicle>(Pawn));
		Distances.Add(GetDistanceAlongSplineForWorldLocation(Pawn->GetActorLocation()));
	}

	for (int Index = 0; Index < VehicleArray.Num(); Index++)
	{
		if (Distances[0] == Distances[1])
		{
			VehicleArray[0]->SetPlayerPlace(1);
			VehicleArray[1]->SetPlayerPlace(1);
		}
		else if (Distances[0] < Distances[1])
		{
			VehicleArray[0]->SetPlayerPlace(1);
			VehicleArray[1]->SetPlayerPlace(2);
		}
		else
		{
			VehicleArray[0]->SetPlayerPlace(2);
			VehicleArray[1]->SetPlayerPlace(1);
		}
	}

	//if (Distances.Num() >= 1)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("1 player: %i"), FMath::RoundToInt(Distances[0])));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("2 player: %i"), FMath::RoundToInt(Distances[1])));
	//}

	Distances.Empty();
}

float ASplineActor::GetDistanceAlongSplineForWorldLocation(FVector Location) const
{
	const float ClosestInputKey = NewSplineComponent->FindInputKeyClosestToWorldLocation(Location);
	const int32 PreviousPoint = FMath::TruncToInt(ClosestInputKey);

	float Distance = NewSplineComponent->GetDistanceAlongSplineAtSplinePoint(PreviousPoint);
	Distance += (ClosestInputKey - PreviousPoint) * (NewSplineComponent->GetDistanceAlongSplineAtSplinePoint(PreviousPoint + 1) - Distance);

	for (int32 i = 0; i < 50; ++i)
	{
		const float InputKeyAtDistance = NewSplineComponent->SplineCurves.ReparamTable.Eval(Distance, 0.0f);

		const float Delta = (NewSplineComponent->GetLocationAtSplineInputKey(InputKeyAtDistance, ESplineCoordinateSpace::World) - NewSplineComponent->GetLocationAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World)).Size();
		if (InputKeyAtDistance < ClosestInputKey)
		{
			Distance += Delta;
		}
		else if (InputKeyAtDistance > ClosestInputKey)
		{
			Distance -= Delta;
		}
		else
		{
			break;
		}
	}

	return FMath::Clamp(Distance, 0.0f, NewSplineComponent->GetSplineLength());
}
