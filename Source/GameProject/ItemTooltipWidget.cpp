// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTooltipWidget.h"
#include "Components/TextBlock.h"
#include "ABGameInstance.h"

void UItemTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NameText = Cast<UTextBlock>(GetWidgetFromName(FName("Name")));
	ItemTypeText = Cast<UTextBlock>(GetWidgetFromName(FName("ItemType")));
	DamageText = Cast<UTextBlock>(GetWidgetFromName(FName("Damage")));
	ArmorText = Cast<UTextBlock>(GetWidgetFromName(FName("Armor")));
	HPText = Cast<UTextBlock>(GetWidgetFromName(FName("HP")));
	SPText = Cast<UTextBlock>(GetWidgetFromName(FName("SP")));
	STRText = Cast<UTextBlock>(GetWidgetFromName(FName("STR")));
	AGIText = Cast<UTextBlock>(GetWidgetFromName(FName("AGI")));
	CONText = Cast<UTextBlock>(GetWidgetFromName(FName("CON")));
	DescriptionText = Cast<UTextBlock>(GetWidgetFromName(FName("Description")));
	PriceText = Cast<UTextBlock>(GetWidgetFromName(FName("Price")));
	WeightText = Cast<UTextBlock>(GetWidgetFromName(FName("Weight")));
}

void UItemTooltipWidget::SetItemToolTip(FItemData& ItemData)
{
	switch (ItemData.GetItemType())
	{
	case EItemType::Etc:
		NameText->SetText(FText::FromName(ItemData.GetName()));
		ItemTypeText->SetText(FText::FromString("Etc"));
		DescriptionText->SetText(FText::FromString(ItemData.GetDescription()));
		PriceText->SetText(FText::FromString(FString::FromInt(ItemData.GetSale())));
		WeightText->SetText(FText::FromString(FString::FromInt(ItemData.GetWeight())));
		
		DamageText->RemoveFromParent();
		ArmorText->RemoveFromParent();
		HPText->RemoveFromParent();
		SPText->RemoveFromParent();
		STRText->RemoveFromParent();
		AGIText->RemoveFromParent();
		CONText->RemoveFromParent();
		break;
	case EItemType::Equipment:
		NameText->SetText(FText::FromName(ItemData.GetName()));
		ItemTypeText->SetText(FText::FromString("Equipment"));
		if (ItemData.GetDamage() != 0)
			DamageText->SetText(FText::FromString(FString(FString::FromInt(ItemData.GetDamage()) + TEXT(" 데미지"))));
		else
			DamageText->RemoveFromParent();
		if (ItemData.GetArmor() != 0)
			ArmorText->SetText(FText::FromString(FString(FString::FromInt(ItemData.GetArmor()) + TEXT(" 방어력"))));
		else
			ArmorText->RemoveFromParent();
		if (ItemData.GetHP() != 0)
			HPText->SetText(FText::FromString(FString(TEXT("+ ") + FString::FromInt(ItemData.GetHP()) + TEXT(" HP"))));
		else
			HPText->RemoveFromParent();
		if (ItemData.GetSP() != 0)
			SPText->SetText(FText::FromString(FString(TEXT("+ ") + FString::FromInt(ItemData.GetSP()) + TEXT(" SP"))));
		else
			SPText->RemoveFromParent();
		if (ItemData.GetSTR() != 0)
			STRText->SetText(FText::FromString(FString(TEXT("+ ") + FString::FromInt(ItemData.GetSTR()) + TEXT(" STR"))));
		else
			STRText->RemoveFromParent();
		if (ItemData.GetAGI() != 0)
			AGIText->SetText(FText::FromString(FString(TEXT("+ ") + FString::FromInt(ItemData.GetAGI()) + TEXT(" AGI"))));
		else
			AGIText->RemoveFromParent();
		if (ItemData.GetCON() != 0)
			CONText->SetText(FText::FromString(FString(TEXT("+ ") + FString::FromInt(ItemData.GetCON()) + TEXT(" CON"))));
		else
			CONText->RemoveFromParent();
		DescriptionText->SetText(FText::FromString(ItemData.GetDescription()));
		PriceText->SetText(FText::FromString(FString::FromInt(ItemData.GetSale())));
		WeightText->SetText(FText::FromString(FString::FromInt(ItemData.GetWeight())));
		break;
	case EItemType::Material:
		NameText->SetText(FText::FromName(ItemData.GetName()));
		ItemTypeText->SetText(FText::FromString(TEXT("재료")));
		DescriptionText->SetText(FText::FromString(ItemData.GetDescription()));
		PriceText->SetText(FText::FromString(FString::FromInt(ItemData.GetSale())));

		DamageText->RemoveFromParent();
		ArmorText->RemoveFromParent();
		HPText->RemoveFromParent();
		SPText->RemoveFromParent();
		STRText->RemoveFromParent();
		AGIText->RemoveFromParent();
		CONText->RemoveFromParent();
		WeightText->RemoveFromParent();
		break;
	default:
		break;
	}
}