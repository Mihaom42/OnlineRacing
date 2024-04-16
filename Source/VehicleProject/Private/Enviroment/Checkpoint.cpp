// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/Checkpoint.h"
#include "Components/BoxComponent.h"

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
	Super::BeginPlay();	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
