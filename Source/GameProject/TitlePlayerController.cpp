// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "Blueprint/UserWidget.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass)
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	}

	UIWidgetInstance->AddToViewport();
	bShowMouseCursor = true;
}