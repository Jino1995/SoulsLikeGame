// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBar.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UBossHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindMonsterStat(class AEnemy* Enemy);

	void UpdateDamageText(float NewDamage);

	void HiddenDamageText();

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	class UTextBlock* BossName;

	UTextBlock* Damage;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class AEnemy* TargetBoss;

	FTimerHandle DamageTextTimer;
};
