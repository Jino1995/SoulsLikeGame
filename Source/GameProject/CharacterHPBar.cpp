// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHPBar.h"
#include "CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCharacterHPBar::BindCharacterStat(class UCharacterStatComponent* NewCharacterStat)
{
	CurrentTxt = Cast<UTextBlock>(GetWidgetFromName(FName("CurrentText")));
	MaxTxt = Cast<UTextBlock>(GetWidgetFromName(FName("MaxText")));
	Bar = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar")));

	CharacterStat = NewCharacterStat;
	CharacterStat->OnMaxHealthChanged.AddLambda([this]()-> void {
		MaxTxt->SetText(FText::FromString(FString::FromInt(CharacterStat->GetMaxHealth())));
		PercentUpdate();
	});

	CharacterStat->OnCurrentHealthChanged.AddLambda([this]()-> void {
		CurrentTxt->SetText(FText::FromString(FString::FromInt(CharacterStat->GetCurrentHealth())));
		PercentUpdate();
	});
	
}

void UCharacterHPBar::PercentUpdate()
{
	Bar->SetPercent(CharacterStat->GetCurrentHealth() / CharacterStat->GetMaxHealth());
}
