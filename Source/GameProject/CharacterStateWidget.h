// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UCharacterStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* LvText;
	class UCharacterStatComponent* CharacterStat;

	class UCharacterHPBar* HPBar;
	class UCharacterSPBar* SPBar;

public:
	void BindCharacterStat(class UCharacterStatComponent* NewCharacterStat);
};
