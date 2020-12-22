// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemyHPBar.h"
#include "Enemy.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

void UNormalEnemyHPBar::BindCharacterStat(AEnemy* Enemy)
{
	WidgetOwner = Enemy;
	WidgetOwner->OnHPChanged.AddUObject(this, &UNormalEnemyHPBar::UpdateHPWidget);
}

void UNormalEnemyHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UNormalEnemyHPBar::UpdateHPWidget()
{
	if (WidgetOwner.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(WidgetOwner->GetCurrentHealth() / WidgetOwner->GetMaxHealth());
		}
	}
}