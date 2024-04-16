#include "VehiclePawn/Vehicle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "VehiclePawn/VehicleController.h"
#include "VehiclePawn/VMovementComponent.h"
#include "Enviroment/Checkpoint.h"
#include "VehicleGameState.h"
#include "Kismet/GameplayStatics.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	RootComponent = CarMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	CameraComponent->FieldOfView = 90.f;

	MovementComp = CreateDefaultSubobject<UVMovementComponent>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UVMovementComponentReplicator>(TEXT("MovementReplicator"));

	PlayerPlace = 1;
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	CarMesh->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::OnOverlap);
	StartPosition = GetTransform().GetTranslation();
}

void AVehicle::Tick(float Delta)
{
	Super::Tick(Delta);

	UpdateHUDStrings();
}

void AVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehicle::MoveRight);
}

void AVehicle::MoveForward(float Value)
{
	if (MovementComp != nullptr)
	{
		MovementComp->SetThrottle(Value);
	}
}

void AVehicle::MoveRight(float Value)
{
	if (MovementComp != nullptr)
	{
		MovementComp->SetSteeringThrow(Value);
	}
}
#define LOCTEXT_NAMESPACE "Vehicle"
void AVehicle::UpdateHUDStrings()
{
	if (MovementComp == nullptr)
	{
		return;
	}

	int32 RoundSpeedValue = MovementComp->GetVelocity().Length();

	SpeedString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(RoundSpeedValue));
}
#undef LOCTEXT_NAMESPACE

void AVehicle::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACheckpoint* OverlapedActor = Cast<ACheckpoint>(OtherActor);

	if (OverlapedActor != nullptr)
	{
		AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
		if (GameState != nullptr)
		{
			AVehicleGameState* VehicleGameState = Cast<AVehicleGameState>(GameState);

			if (VehicleGameState != nullptr)
			{
				VehicleGameState->MarkFinishPlayer(PlayerName);
			}
		}
	}
}