// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyWidget.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

void UMoneyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Money = Cast<UTextBlock>(GetWidgetFromName(FName("MoneyText")));

	CharacterInventory = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory();
	CharacterInventory->OnMoneyChanged.AddLambda([this]()->void {
		Money->SetText(FText::FromString(FString::FromInt(CharacterInventory->GetCurrentMoney())));
	});

}