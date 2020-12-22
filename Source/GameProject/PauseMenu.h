// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UButton* InventoryBt;

	UPROPERTY()
	UButton* StatBt;

	UPROPERTY()
	UButton* BackBt;

	UPROPERTY()
	UButton* MainMenuBt;

public:
	UFUNCTION()
	void ClickedInventoryBt();

	UFUNCTION()
	void ClickedStatBt();

	UFUNCTION()
	void ClickedBackBt();

	UFUNCTION()
	void ClickedMainMenuBt();
};