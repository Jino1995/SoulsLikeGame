// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormalEnemyHPBar.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UNormalEnemyHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class AEnemy* Enemy);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class AEnemy> WidgetOwner;

	UPROPERTY()
	class UProgressBar* HPProgressBar;


};
