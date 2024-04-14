// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class VEHICLEPROJECT_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameOverText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* YesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* NoButton;
};
