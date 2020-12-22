// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateWidget.h"
#include "Components/TextBlock.h"
#include "CharacterStatComponent.h"
#include "CharacterHPBar.h"
#include "CharacterSPBar.h"

void UCharacterStateWidget::BindCharacterStat(class UCharacterStatComponent* NewCharacterStat)
{
	LvText = Cast<UTextBlock>(GetWidgetFromName(FName("LevelText")));

	CharacterStat = NewCharacterStat;
	
	CharacterStat->OnLevelChanged.AddLambda([this]()-> void {
		LvText->SetText(FText::FromString(FString::FromInt(CharacterStat->GetLevel())));
	});
	
	HPBar = Cast<UCharacterHPBar>(GetWidgetFromName(FName("HealthBar")));
	SPBar = Cast<UCharacterSPBar>(GetWidgetFromName(FName("StaminaBar")));
	
	HPBar->BindCharacterStat(CharacterStat);
	SPBar->BindCharacterStat(CharacterStat);
}
