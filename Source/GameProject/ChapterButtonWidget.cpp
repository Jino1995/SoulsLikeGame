// Fill out your copyright notice in the Description page of Project Settings.


#include "ChapterButtonWidget.h"
#include "PortalDetailWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PortalWidget.h"

void UChapterButtonWidget::NativeConstruct()
{
	SlotBt = Cast<UButton>(GetWidgetFromName(FName("SlotButton")));
	SlotBt->OnClicked.AddDynamic(this, &UChapterButtonWidget::ClickedChapterButton);

	ChapterName = Cast<UTextBlock>(GetWidgetFromName(FName("ChapterNameText")));
	ChapterName->SetText(FText::FromString(ChapterNameTxt));
	
	SlotBt->WidgetStyle.Normal.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void UChapterButtonWidget::SetParentWidget(UPortalWidget* NewParentWidget)
{
	ParentWidget = NewParentWidget;
}

void UChapterButtonWidget::ClickedChapterButton()
{
	if (ParentWidget->OnClickedChapterButton(this))
	{
		SlotBt->WidgetStyle.Normal.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void UChapterButtonWidget::ResetWidgetState()
{
	SlotBt->WidgetStyle.Normal.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);
}