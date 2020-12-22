// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemTextContainerWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void USystemTextContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Canvas = Cast<UCanvasPanel>(GetWidgetFromName(FName("Canvas")));
}

void USystemTextContainerWidget::CreateAddItemText(FName NewItemName)
{
	auto ItemText = CreateWidget<UUserWidget>(GetOwningPlayer(), AddItemText);
	Canvas->AddChildToCanvas(ItemText);
	Cast<UTextBlock>(ItemText->GetWidgetFromName(FName("ItemName")))->SetText(FText::FromString(NewItemName.ToString()));
}

void USystemTextContainerWidget::CreateInventoryFullText()
{
	auto InventoryFull = CreateWidget<UUserWidget>(GetOwningPlayer(), InventoryFullText);
	Canvas->AddChildToCanvas(InventoryFull);
}

void USystemTextContainerWidget::CreateWeightFullText()
{
	auto WeightFull = CreateWidget<UUserWidget>(GetOwningPlayer(), WeightFullText);
	Canvas->AddChildToCanvas(WeightFull);
}