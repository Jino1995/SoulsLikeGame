// Fill out your copyright notice in the Description page of Project Settings.


#include "HPFloatingText.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UHPFloatingText::NativeConstruct()
{
	Super::NativeConstruct();
	Canvas = Cast<UCanvasPanel>(GetWidgetFromName(FName("Canvas")));
}

void UHPFloatingText::CreateDamageText(int32 Damage)
{
	auto DText = CreateWidget<UUserWidget>(GetOwningPlayer(), DamageTextAsset);
	Canvas->AddChildToCanvas(DText);
	Cast<UTextBlock>(DText->GetWidgetFromName(FName("TxtBlock")))->SetText(FText::FromString(FString::FromInt(Damage)));
}

void UHPFloatingText::CreateMoneyText(int32 Money)
{
	auto MText = CreateWidget<UUserWidget>(GetOwningPlayer(), MoneyTextAsset);
	Canvas->AddChildToCanvas(MText);
	Cast<UTextBlock>(MText->GetWidgetFromName(FName("TxtBlock")))->SetText(FText::FromString(FString::FromInt(Money)));
}

void UHPFloatingText::CreateHealingText(int32 Heal)
{
	auto HText = CreateWidget<UUserWidget>(GetOwningPlayer(), HealingTextAsset);
	Canvas->AddChildToCanvas(HText);
	Cast<UTextBlock>(HText->GetWidgetFromName(FName("TxtBlock")))->SetText(FText::FromString(FString::FromInt(Heal)));
}
