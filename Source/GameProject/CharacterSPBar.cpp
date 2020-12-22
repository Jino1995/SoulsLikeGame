// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSPBar.h"
#include "CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCharacterSPBar::BindCharacterStat(class UCharacterStatComponent* NewCharacterStat)
{
	CurrentTxt = Cast<UTextBlock>(GetWidgetFromName(FName("CurrentText")));
	MaxTxt = Cast<UTextBlock>(GetWidgetFromName(FName("MaxText")));
	Bar = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar")));

	CharacterStat = NewCharacterStat;

	CharacterStat->OnMaxStaminaChanged.AddLambda([this]()-> void {
		MaxTxt->SetText(FText::FromString(FString::FromInt(CharacterStat->GetMaxStamina())));
		PercentUpdate();
	});


	CharacterStat->OnCurrentStaminaChanged.AddLambda([this]()-> void {
		CurrentTxt->SetText(FText::FromString(FString::FromInt(CharacterStat->GetCurrentStamina())));
		PercentUpdate();
	});
}

void UCharacterSPBar::PercentUpdate()
{
	Bar->SetPercent(CharacterStat->GetCurrentStamina() / CharacterStat->GetMaxStamina());
}
