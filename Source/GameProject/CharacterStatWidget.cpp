// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatWidget.h"
#include "CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCharacterStatWidget::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	/*
	Level = Cast<UTextBlock>(GetWidgetFromName(FName("LevelText")));
	HPPoint = Cast<UTextBlock>(GetWidgetFromName(FName("HPPointText")));
	SPPoint = Cast<UTextBlock>(GetWidgetFromName(FName("SPPointText")));
	STRPoint = Cast<UTextBlock>(GetWidgetFromName(FName("STRPointText")));
	AGIPoint = Cast<UTextBlock>(GetWidgetFromName(FName("AGIPointText")));
	CONPoint = Cast<UTextBlock>(GetWidgetFromName(FName("CONPointText")));
	StatPoint = Cast<UTextBlock>(GetWidgetFromName(FName("StatPointNum")));

	TotalHealth = Cast<UTextBlock>(GetWidgetFromName(FName("TotalHealthText")));
	TotalStamina = Cast<UTextBlock>(GetWidgetFromName(FName("TotalStaminaText")));
	TotalAttack = Cast<UTextBlock>(GetWidgetFromName(FName("TotalAttackText")));
	TotalDefense = Cast<UTextBlock>(GetWidgetFromName(FName("TotalDefenseText")));

	HPPointBt = Cast<UButton>(GetWidgetFromName(FName("HPUpButton")));
	SPPointBt = Cast<UButton>(GetWidgetFromName(FName("SPUpButton")));
	STRPointBt = Cast<UButton>(GetWidgetFromName(FName("STRUpButton")));
	AGIPointBt = Cast<UButton>(GetWidgetFromName(FName("AGIUpButton")));
	CONPointBt = Cast<UButton>(GetWidgetFromName(FName("CONUpButton")));
	
	HPPointBt->OnClicked.AddDynamic(this, &UCharacterStatWidget::ClickedHPPoint);
	SPPointBt->OnClicked.AddDynamic(this, &UCharacterStatWidget::ClickedSPPoint);
	STRPointBt->OnClicked.AddDynamic(this, &UCharacterStatWidget::ClickedSTRPoint);
	AGIPointBt->OnClicked.AddDynamic(this, &UCharacterStatWidget::ClickedAGIPoint);
	CONPointBt->OnClicked.AddDynamic(this, &UCharacterStatWidget::ClickedCONPoint);

	HPPointBt->SetVisibility(ESlateVisibility::Hidden);
	SPPointBt->SetVisibility(ESlateVisibility::Hidden);
	STRPointBt->SetVisibility(ESlateVisibility::Hidden);
	AGIPointBt->SetVisibility(ESlateVisibility::Hidden);
	CONPointBt->SetVisibility(ESlateVisibility::Hidden);

	CharacterStat = NewCharacterStat;
	CharacterStat->OnLevelChanged.AddLambda([this]()->void {
		Level->SetText(FText::FromString(FString::FromInt(CharacterStat->GetLevel())));
	});

	CharacterStat->OnMaxHealthChanged.AddLambda([this]()->void {
		TotalHealth->SetText(FText::FromString(FString::FromInt(CharacterStat->GetMaxHealth())));
	});

	CharacterStat->OnMaxStaminaChanged.AddLambda([this]()->void {
		TotalStamina->SetText(FText::FromString(FString::FromInt(CharacterStat->GetMaxStamina())));
	});

	CharacterStat->OnAttackChanged.AddLambda([this]()->void {
		TotalAttack->SetText(FText::FromString(FString::FromInt(CharacterStat->GetAttack())));
	});

	CharacterStat->OnDefenseChanged.AddLambda([this]()->void {
		TotalDefense->SetText(FText::FromString(FString::FromInt(CharacterStat->GetDefense())));
	});

	CharacterStat->OnHPStatChanged.AddLambda([this]()->void {
		HPPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetHPStat())));
	});

	CharacterStat->OnSPStatChanged.AddLambda([this]()->void {
		SPPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetSPStat())));
	});

	CharacterStat->OnSTRStatChanged.AddLambda([this]()->void {
		STRPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetSTRStat())));
	});
	
	CharacterStat->OnAGIStatChanged.AddLambda([this]()->void {
		AGIPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetAGIStat())));
	});

	CharacterStat->OnCONStatChanged.AddLambda([this]()->void {
		CONPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetCONStat())));
	});

	CharacterStat->OnStatPointChanged.AddLambda([this]()->void {
		if (CharacterStat->GetStatPoint() == 0)
		{
			HPPointBt->SetVisibility(ESlateVisibility::Hidden);
			SPPointBt->SetVisibility(ESlateVisibility::Hidden);
			STRPointBt->SetVisibility(ESlateVisibility::Hidden);
			AGIPointBt->SetVisibility(ESlateVisibility::Hidden);
			CONPointBt->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (HPPointBt->GetVisibility() == ESlateVisibility::Hidden)
		{
			HPPointBt->SetVisibility(ESlateVisibility::Visible);
			SPPointBt->SetVisibility(ESlateVisibility::Visible);
			STRPointBt->SetVisibility(ESlateVisibility::Visible);
			AGIPointBt->SetVisibility(ESlateVisibility::Visible);
			CONPointBt->SetVisibility(ESlateVisibility::Visible);
		}
		StatPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetStatPoint())));
	});
	*/
}

void UCharacterStatWidget::ClickedHPPoint()
{
	//CharacterStat->UseStatPoint(FString("HP"));
}

void UCharacterStatWidget::ClickedSPPoint()
{
	//CharacterStat->UseStatPoint(FString("SP"));
}

void UCharacterStatWidget::ClickedSTRPoint()
{
	//CharacterStat->UseStatPoint(FString("ATK"));
}

void UCharacterStatWidget::ClickedAGIPoint()
{
	//CharacterStat->UseStatPoint(FString("ATK"));
}

void UCharacterStatWidget::ClickedCONPoint()
{
	//CharacterStat->UseStatPoint(FString("ATK"));
}