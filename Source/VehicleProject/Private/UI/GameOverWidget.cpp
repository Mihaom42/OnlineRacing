// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"
#include "Components/Button.h"
#include "VehiclePawn/VehiclePlayerState.h"
#include "Kismet/GameplayStatics.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NoButton != nullptr)
	{
		NoButton->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGame);
	}

	if (YesButton != nullptr)
	{
		YesButton->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);
	}
}

void UGameOverWidget::RestartGame()
{
	APlayerController* PlayerController = GetOwningPlayer();
	
	if (PlayerController != nullptr)
	{
		APlayerState* PlayerState = PlayerController->PlayerState;

		if (PlayerState != nullptr)
		{
			AVehiclePlayerState* VehiclePlayerState = Cast<AVehiclePlayerState>(PlayerState);

			if (VehiclePlayerState != nullptr)
			{
				VehiclePlayerState->SetPlayAgain(true);
			}
		}
	}
}

void UGameOverWidget::QuitGame()
{
	UWorld* const World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ConsoleCommand("quit");
}