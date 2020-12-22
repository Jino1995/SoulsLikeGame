// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:

private:
	class UTextBlock* Level;
	UTextBlock* LevelUpCost;
	UTextBlock* LevelUpUnit;
	UTextBlock* HPPoint;
	UTextBlock* SPPoint;
	UTextBlock* STRPoint;
	UTextBlock* AGIPoint;
	UTextBlock* StatPoint;

	UTextBlock* TotalHealth;
	UTextBlock* TotalStamina;
	UTextBlock* TotalAttack;
	UTextBlock* TotalDefense;

	class UButton* LevelUpBt;
	UButton* BackBt;
	UButton* HPPointBt;
	UButton* SPPointBt;
	UButton* STRPointBt;
	UButton* AGIPointBt;

	class UCharacterStatComponent* CharacterStat;

	UFUNCTION()
	void ClickedHPPoint();

	UFUNCTION()
	void ClickedSPPoint();

	UFUNCTION()
	void ClickedSTRPoint();

	UFUNCTION()
	void ClickedAGIPoint();
	
	UFUNCTION()
	void ClickedBackBt();

	UFUNCTION()
	void ClickedLevelUpBt();
};
