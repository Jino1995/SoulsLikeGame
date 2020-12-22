// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionSlot.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"

void UPotionSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PotionAmount = Cast<UTextBlock>(GetWidgetFromName(FName("PotionNumText")));
	
}

void UPotionSlot::BindInventoryComp(UInventoryComponent* NewInventoryComp)
{
	InventoryComp = NewInventoryComp;

	InventoryComp->OnPotionNumChanged.AddLambda([this]() -> void {
		PotionAmount->SetText(FText::FromString(FString::FromInt(InventoryComp->GetCurrentPotionNum())));
	});
}