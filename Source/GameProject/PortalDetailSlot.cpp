// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalDetailSlot.h"
#include "PortalDetailWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UPortalDetailSlot::NativeConstruct()
{
	SlotBt = Cast<UButton>(GetWidgetFromName(FName("SlotButton")));

	PortLevelName = Cast<UTextBlock>(GetWidgetFromName(FName("LevelName")));
	PortLevelName->SetText(FText::FromString(PortLevelNameTxt));

	SlotBt->OnClicked.AddDynamic(this, &UPortalDetailSlot::ClickedPortalSlot);
}

void UPortalDetailSlot::SetParentWidget(UPortalDetailWidget* NewParentWidget)
{
	ParentWidget = NewParentWidget;
}

void UPortalDetailSlot::ClickedPortalSlot()
{
	ParentWidget->ClickedPortListSlot(this);
}