// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABGameInstance.h"
#include "ItemTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UTextBlock* NameText;

	UPROPERTY()
		class UTextBlock* ItemTypeText;

	UPROPERTY()
		class UTextBlock* DamageText;

	UPROPERTY()
		class UTextBlock* ArmorText;

	UPROPERTY()
		class UTextBlock* HPText;

	UPROPERTY()
		class UTextBlock* SPText;

	UPROPERTY()
		class UTextBlock* STRText;

	UPROPERTY()
		class UTextBlock* AGIText;

	UPROPERTY()
		class UTextBlock* CONText;

	UPROPERTY()
		class UTextBlock* DescriptionText;

	UPROPERTY()
		class UTextBlock* PriceText;

	UPROPERTY()
		class UTextBlock* WeightText;

public:
	void SetItemToolTip(struct FItemData& ItemData);

};
