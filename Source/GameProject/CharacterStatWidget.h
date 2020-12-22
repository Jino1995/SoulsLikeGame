// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnIncreaseHPStatDelegate);
DECLARE_MULTICAST_DELEGATE(FOnIncreaseSPStatDelegate);
DECLARE_MULTICAST_DELEGATE(FOnIncreaseSTRStatDelegate);
DECLARE_MULTICAST_DELEGATE(FOnIncreaseAGIStatDelegate);
DECLARE_MULTICAST_DELEGATE(FOnIncreaseCONStatDelegate);

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UCharacterStatComponent* NewCharacterStat);

private:
	class UTextBlock* Level;
	UTextBlock* HPPoint;
	UTextBlock* SPPoint;
	UTextBlock* STRPoint;
	UTextBlock* AGIPoint;
	UTextBlock* CONPoint;
	UTextBlock* StatPoint;

	UTextBlock* TotalHealth;
	UTextBlock* TotalStamina;
	UTextBlock* TotalAttack;
	UTextBlock* TotalDefense;

	class UButton* HPPointBt;
	UButton* SPPointBt;
	UButton* STRPointBt;
	UButton* AGIPointBt;
	UButton* CONPointBt;

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
	void ClickedCONPoint();
};
