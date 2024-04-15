// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/Checkpoint.h"
#include "Components/BoxComponent.h"
#include "VehiclePawn/Vehicle.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlap);

	Super::BeginPlay();	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVehicle* OverlapPawn = Cast<AVehicle>(OtherActor);

	if (OverlapPawn != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("FINISH!")));
	}
}