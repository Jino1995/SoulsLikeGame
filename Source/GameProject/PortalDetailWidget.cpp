// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalDetailWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PortalDetailSlot.h"
#include "ChapterButtonWidget.h"

void UPortalDetailWidget::NativeConstruct()
{
	PortalSlotList = Cast<UVerticalBox>(GetWidgetFromName(FName("PortalList")));

	LevelImage = Cast<UImage>(GetWidgetFromName(FName("PortImage")));

	PortLevelDetail = Cast<UTextBlock>(GetWidgetFromName(FName("PortText")));

	for (auto PortSlot : PortalSlotList->GetAllChildren())
	{
		Cast<UPortalDetailSlot>(PortSlot)->SetParentWidget(this);
	}
}

void UPortalDetailWidget::ClickedPortListSlot(UPortalDetailSlot* ClickedSlot)
{
	PortTarget = ClickedSlot;
	LevelImage->SetBrushFromTexture(PortTarget->GetLevelImg());
	PortLevelDetail->SetText(FText::FromString(PortTarget->GetDetail()));
}