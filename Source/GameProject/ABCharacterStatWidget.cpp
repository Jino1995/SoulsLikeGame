// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ABPlayerController.h"
#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "ABGameInstance.h"
#include "InventoryComponent.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackBt = Cast<UButton>(GetWidgetFromName(FName("BackButton")));
	BackBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedBackBt);

	LevelUpBt = Cast<UButton>(GetWidgetFromName(FName("LevelUpButton")));
	LevelUpBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedLevelUpBt);

	Level = Cast<UTextBlock>(GetWidgetFromName(FName("LevelText")));
	LevelUpCost = Cast<UTextBlock>(GetWidgetFromName(FName("LevelUpCostText")));
	LevelUpUnit = Cast<UTextBlock>(GetWidgetFromName(FName("LevelUpUnitText")));
	HPPoint = Cast<UTextBlock>(GetWidgetFromName(FName("HPPointText")));
	SPPoint = Cast<UTextBlock>(GetWidgetFromName(FName("SPPointText")));
	STRPoint = Cast<UTextBlock>(GetWidgetFromName(FName("STRPointText")));
	AGIPoint = Cast<UTextBlock>(GetWidgetFromName(FName("AGIPointText")));
	StatPoint = Cast<UTextBlock>(GetWidgetFromName(FName("StatPointNum")));

	TotalHealth = Cast<UTextBlock>(GetWidgetFromName(FName("TotalHealthText")));
	TotalStamina = Cast<UTextBlock>(GetWidgetFromName(FName("TotalStaminaText")));
	TotalAttack = Cast<UTextBlock>(GetWidgetFromName(FName("TotalAttackText")));
	TotalDefense = Cast<UTextBlock>(GetWidgetFromName(FName("TotalDefenseText")));

	HPPointBt = Cast<UButton>(GetWidgetFromName(FName("HPUpButton")));
	SPPointBt = Cast<UButton>(GetWidgetFromName(FName("SPUpButton")));
	STRPointBt = Cast<UButton>(GetWidgetFromName(FName("STRUpButton")));
	AGIPointBt = Cast<UButton>(GetWidgetFromName(FName("AGIUpButton")));

	HPPointBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedHPPoint);
	SPPointBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedSPPoint);
	STRPointBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedSTRPoint);
	AGIPointBt->OnClicked.AddDynamic(this, &UABCharacterStatWidget::ClickedAGIPoint);

	HPPointBt->SetVisibility(ESlateVisibility::Hidden);
	SPPointBt->SetVisibility(ESlateVisibility::Hidden);
	STRPointBt->SetVisibility(ESlateVisibility::Hidden);
	AGIPointBt->SetVisibility(ESlateVisibility::Hidden);
	
	CharacterStat = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetCharacterStat();
	CharacterStat->OnLevelChanged.AddLambda([this]()->void {
		UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		auto LevelUpCostInfo = ABGameInstance->GetLevelUpInfo(CharacterStat->GetLevel());
		
		Level->SetText(FText::FromString(FString::FromInt(CharacterStat->GetLevel())));
		if (LevelUpCostInfo->GetLevelUpCost() < 0)
		{
			LevelUpCost->SetText(FText::FromString("Max Lv    "));
			LevelUpBt->SetVisibility(ESlateVisibility::Hidden);
			LevelUpUnit->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			LevelUpCost->SetText(FText::FromString(FString::FromInt(LevelUpCostInfo->GetLevelUpCost())));
		}
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
	
	CharacterStat->OnStatPointChanged.AddLambda([this]()->void {
		if (CharacterStat->GetStatPoint() == 0)
		{
			HPPointBt->SetVisibility(ESlateVisibility::Hidden);
			SPPointBt->SetVisibility(ESlateVisibility::Hidden);
			STRPointBt->SetVisibility(ESlateVisibility::Hidden);
			AGIPointBt->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			HPPointBt->SetVisibility(ESlateVisibility::Visible);
			SPPointBt->SetVisibility(ESlateVisibility::Visible);
			STRPointBt->SetVisibility(ESlateVisibility::Visible);
			AGIPointBt->SetVisibility(ESlateVisibility::Visible);
		}
		
		StatPoint->SetText(FText::FromString(FString::FromInt(CharacterStat->GetStatPoint())));
	});
}

void UABCharacterStatWidget::ClickedBackBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenCharacterStatWidget();
	Cast<AABPlayerController>(GetOwningPlayer())->VisiblePauseMenu();
}

void UABCharacterStatWidget::ClickedLevelUpBt()
{
	if (Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->DecMoney(FCString::Atoi(*(LevelUpCost->GetText().ToString()))))
	{
		CharacterStat->LevelUp();
	}

}

void UABCharacterStatWidget::ClickedHPPoint()
{
	CharacterStat->UseStatPointForHP();
}

void UABCharacterStatWidget::ClickedSPPoint()
{
	CharacterStat->UseStatPointForSP();
}

void UABCharacterStatWidget::ClickedSTRPoint()
{
	CharacterStat->UseStatPointForSTR();
}

void UABCharacterStatWidget::ClickedAGIPoint()
{
	CharacterStat->UseStatPointForAGI();
}