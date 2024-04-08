#include "VehiclePawn/Vehicle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "VehiclePawn/VehicleController.h"
#include "Net/UnrealNetwork.h"
#include "VehiclePawn/VehicleMovementComponent.h"

AVehicle::AVehicle()
{
	//PrimaryActorTick.bCanEverTick = true;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	//CarMesh->SetSimulatePhysics(true);
	//CarMesh->SetLinearDamping(0.1f);
	RootComponent = CarMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 500.0f;
	//SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bDoCollisionTest = false;

	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;*/

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	
	bReplicates = true;
	//bReplicateMovement = false;

	MovementComponent = CreateDefaultSubobject<UVehicleMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

	/*Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;*/

	MaxMoveSpeed = 400.f;

	bReplicates = true;

}

void AVehicle::BeginPlay()
{
	//Add Input Mapping Context
	/*if (AVehicleController* VehicleController = Cast<AVehicleController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(VehicleController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}*/

	//SetReplicateMovement(true);

	Super::BeginPlay();
}

void AVehicle::Tick(float Delta)
{
	//VehicleVelocity = CarMesh->GetPhysicsLinearVelocity();

	Super::Tick(Delta);
}

void AVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//// Set up action bindings
	//if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	//{
	//	EnhancedInputComponent->BindAction(GasAction, ETriggerEvent::Triggered, this, &AVehicle::MoveForward);
	//	EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AVehicle::PushHandbrake);
	//}

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehicle::MoveRight);
}

void AVehicle::MoveForward(float Value)
{
	//FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent)) 
	{
		MovementComponent->AddInputVector(GetActorForwardVector() * Value);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Y: %f"), Value));
	}

	Server_SendPlayerTransformToServer(this->GetTransform());
}

void AVehicle::MoveRight(float Value)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorRightVector() * Value);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Y: %f"), Value));
	}

	Server_SendPlayerTransformToServer(this->GetTransform());
}

void AVehicle::OnRep_VehicleTransform()
{
	SetActorTransform(VehicleTransform);
}

void AVehicle::Server_SendPlayerTransformToServer_Implementation(FTransform NewTransform)
{
	VehicleTransform = NewTransform;
}

void AVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehicle, VehicleTransform);
}