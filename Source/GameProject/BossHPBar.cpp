// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPBar.h"
#include "Enemy.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"

void UBossHPBar::BindMonsterStat(AEnemy* Enemy)
{
	TargetBoss = Enemy;
	TargetBoss->OnHPChanged.AddUObject(this, &UBossHPBar::UpdateHPWidget);

	BossName->SetText(FText::FromName(TargetBoss->GetMonsterName()));
	Damage->SetVisibility(ESlateVisibility::Hidden);

	TargetBoss->SetBossHPBar(this);
	UpdateHPWidget();

}

void UBossHPBar::UpdateDamageText(float NewDamage)
{
	Damage->SetText(FText::FromString(FString::FromInt(NewDamage)));
	Damage->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(DamageTextTimer, this, &UBossHPBar::HiddenDamageText, 1.0, true);
}

void UBossHPBar::HiddenDamageText()
{
	Damage->SetVisibility(ESlateVisibility::Hidden);
}

void UBossHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));

	Damage = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamageText")));

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UBossHPBar::UpdateHPWidget()
{
	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(TargetBoss->GetCurrentHealth() / TargetBoss->GetMaxHealth());
	}
}